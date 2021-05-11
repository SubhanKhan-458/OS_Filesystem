  
#include "../include/data_blocks.h"
#include "../include/olt.h"
#include<stdlib.h>
#include <stdio.h>

void initialize_data_blocks(int *device_descriptor)
{
    char buffer[1] = {'\0'};
    int i;

    for (i = DATA_BLOCK_NO; i < NO_OF_BLOCKS; i++)
    {
        writeBlock(device_descriptor, (void *)buffer, i);
    }
}

int remaining_block_lookup(int *device_descriptor, inode *inode_buff, int buffer_size, int *remaining_size)
{
    int i, j, datablock_index;
    char read_buff[BLOCK_SIZE];

    for (i = 0; i < DIRECT_BLOCKS; i++)
    {
        datablock_index = inode_buff->index_pointers[i];
        if (datablock_index != -1)
        {
            readBlock(device_descriptor, read_buff, datablock_index);
            j = 0;
            while (read_buff[j] != '\0')
            {
                j++;
            }

            if ((BLOCK_SIZE - j) > buffer_size)
            {
                *remaining_size = j;
                return datablock_index;
            }
        }

        datablock_index = -1;
    }

    single_indirect_block *indirect_node_buff = (single_indirect_block *)malloc(sizeof(single_indirect_block));
    int k;

    for (; i < (DIRECT_BLOCKS + INDIRECT_DEPTH); i++)
    {
        if (inode_buff->index_pointers[i] != -1)
        {
            // fetch single_indirect node
            read_indirect_node(device_descriptor, indirect_node_buff, inode_buff->index_pointers[i]);

            for (k = 0; k < DIRECT_BLOCKS; k++)
            {
                datablock_index = indirect_node_buff->index_pointers[k];
                if (datablock_index != -1)
                {
                    readBlock(device_descriptor, read_buff, datablock_index);
                    j = 0;
                    while (read_buff[j] != '\0')
                    {
                        j++;
                    }

                    if ((BLOCK_SIZE - j) > buffer_size)
                    {
                        *remaining_size = j;
                        return datablock_index;
                    }
                }

                datablock_index = -1;
            }
        }
    }

    free(indirect_node_buff);

    return -1;
}

int free_block_lookup()
{
    int i;

    for (i = 0; i < NO_OF_DATA_BLOCKS; i++)
    {
        if (DATA_BITMAP.bitmap[i] == '0')
        {
            return (DATA_BLOCK_NO + i);
        }
    }

    return -1;
}

void write_data(int *device_descriptor, int inode_index, char *buffer, int buffer_size)
{
    inode *inode_buffer = (inode *)malloc(sizeof(inode));
    char write_buffer[BLOCK_SIZE]; // we portion the buffer in BLOCK_SIZE chunks

    int remaining_bytes = buffer_size, blocks_written = 0, data_block_index;
    int i, j, write_block_flag = 0, empty_indirect_node;

    single_indirect_block *indirect_node_buffer;

    while (remaining_bytes >= BLOCK_SIZE)
    {
        memcpy(write_buffer, buffer + (blocks_written * (BLOCK_SIZE)), (BLOCK_SIZE - 1));
        write_buffer[BLOCK_SIZE] = '\0';

        remaining_bytes -= (BLOCK_SIZE - 1);

        for (i = 0; i < DIRECT_BLOCKS; i++)
        {
            if (inode_buffer->index_pointers[i] == -1)
            {
                data_block_index = free_block_lookup();
                inode_buffer->index_pointers[i] = data_block_index;
                DATA_BITMAP.bitmap[i] = '1';
                printf("%d\n",data_block_index);
                write_inode(device_descriptor, inode_buffer, inode_index);
                writeBlock(device_descriptor, write_buffer, data_block_index);

                blocks_written++;
                write_block_flag = 1;
                break;
            }
        }

        if (write_block_flag == 1)
        {
            write_block_flag = 0;
            continue;
        }

        for (i; i < INDIRECT_DEPTH + DIRECT_BLOCKS; i++)
        {
            if (inode_buffer->index_pointers[i] == -1)
            {
                indirect_node_buffer = (single_indirect_block *)malloc(sizeof(single_indirect_block));
                empty_indirect_node = get_empty_index_block_index();
                data_block_index = free_block_lookup();
                indirect_node_buffer->index_pointers[0] = data_block_index;
                write_indirect_node(device_descriptor, indirect_node_buffer, empty_indirect_node);
                inode_buffer->index_pointers[i] = empty_indirect_node;
                write_inode(device_descriptor, inode_buffer, inode_index);
                writeBlock(device_descriptor, write_buffer, data_block_index);

                blocks_written++;
                write_block_flag = 1;
                free(indirect_node_buffer);
                break;
            }
            else
            {
                indirect_node_buffer = (single_indirect_block *)malloc(sizeof(single_indirect_block));
                read_indirect_node(device_descriptor, indirect_node_buffer, inode_buffer->index_pointers[i]);
                for (j = 0; j < DIRECT_BLOCKS; j++)
                {
                    if (indirect_node_buffer->index_pointers[j] == -1)
                    {
                        data_block_index = free_block_lookup();
                        writeBlock(device_descriptor, write_buffer, data_block_index);

                        blocks_written++;
                        write_block_flag = 1;
                        break;
                    }
                }

                if (write_block_flag == 1)
                {
                    break;
                }
                free(indirect_node_buffer);
            }
        }

        if (write_block_flag != 1)
        {
            continue; // problem writing
        }
    }

    int *available_bytes = (int *)malloc(sizeof(int));
    char *remaining_buff = (char *)malloc(sizeof(char) * remaining_bytes + 1);
    data_block_index = remaining_block_lookup(device_descriptor, inode_buffer, (remaining_bytes + 1), available_bytes);
    printf("%d\n",data_block_index);
    memcpy(remaining_buff, buffer + (blocks_written * BLOCK_SIZE), remaining_bytes);
    remaining_buff[(sizeof(char) * remaining_bytes)] = '\0';
    if (data_block_index != -1)
    {
        readBlock(device_descriptor, write_buffer, data_block_index);
        memcpy(write_buffer + (*available_bytes), remaining_buff, remaining_bytes);
        return;
    }
    else
    {
        for (i = 0; i < DIRECT_BLOCKS; ++i)
        {
            if (inode_buffer->index_pointers[i] == -1)
            {
                data_block_index = free_block_lookup();
                inode_buffer->index_pointers[i] = data_block_index;
                DATA_BITMAP.bitmap[data_block_index] = '1';
                printf("%d\n",data_block_index);
                write_inode(device_descriptor, inode_buffer, inode_index);
                writeBlock(device_descriptor, remaining_buff, data_block_index);
                return;
            }
        }
        for (i; i < DIRECT_BLOCKS + INDIRECT_DEPTH; ++i)
        {

            if (inode_buffer->index_pointers[i] == -1)
            {
                indirect_node_buffer = (single_indirect_block *)malloc(sizeof(single_indirect_block));
                empty_indirect_node = get_empty_index_block_index();
                data_block_index = free_block_lookup();
                indirect_node_buffer->index_pointers[0] = data_block_index;
                write_indirect_node(device_descriptor, indirect_node_buffer, empty_indirect_node);
                inode_buffer->index_pointers[i] = empty_indirect_node;
                write_inode(device_descriptor, inode_buffer, inode_index);
                writeBlock(device_descriptor, remaining_buff, data_block_index);
                free(indirect_node_buffer);
                return;
            }
            else
            {
                indirect_node_buffer = (single_indirect_block *)malloc(sizeof(single_indirect_block));
                read_indirect_node(device_descriptor, indirect_node_buffer, inode_buffer->index_pointers[i]);
                for (j = 0; j < DIRECT_BLOCKS; j++)
                {
                    if (indirect_node_buffer->index_pointers[j] == -1)
                    {
                        data_block_index = free_block_lookup();
                        indirect_node_buffer->index_pointers[j] = data_block_index;
                        write_indirect_node(device_descriptor, indirect_node_buffer, empty_indirect_node);
                        inode_buffer->index_pointers[i] = empty_indirect_node;
                        write_inode(device_descriptor, inode_buffer, inode_index);        
                        writeBlock(device_descriptor, remaining_buff, data_block_index);
                        free(indirect_node_buffer);
                        return;
                    }
                }
                free(indirect_node_buffer);
            }
        }
    }
}

void read_data(int *device_descriptor, int inode_index, char *buffer, int buffer_size)
{
    inode* buff = (inode*)malloc(sizeof (inode));

    read_inode(device_descriptor,buff,inode_index);

    
    int i=0;
    for(i;i<DIRECT_BLOCKS;i++){
        if(buff->index_pointers[i]!=-1){
            readBlock(device_descriptor,(void*)buffer,buff->index_pointers[i]);
            printf("%s\n\n",buffer);
        }
    }
    free(buff);

}

int main () {
    int * fd = (int *) malloc(sizeof(int));
    openDisk("foo.txt", fd);


    int i = 0;
    char * block = (char *) malloc(BLOCK_SIZE);

    for (; i < INODE_BLOCK_NO; i++) {
        writeBlock(fd, (void *) block, i);
    }

    initialize_inode_blocks(fd);
    initialize_inode_bitmap(fd);
    initialize_inode_blocks(fd);
    initialize_index_block_bitmap(fd);
    initialize_data_blocks(fd);
    initialize_data_block_bitmap(fd);

    char * buff = "Google was founded in September 1998 by Larry Page and Sergey Brin while they were Ph.D. students at Stanford University in California. Together they own about 14% of its shares and control 56% of the stockholder voting power through super-voting stock. Google was incorporated in California on September 4, 1998. Google was then reincorporated in Delaware on October 22, 2002.[12] In July 2003, Google moved to its headquarters in Mountain View, California, nicknamed the Googleplex. The company became a public company via an initial public offering (IPO) on August 19, 2004. In October 2015, Google reorganized as a subsidiary of a conglomerate called Alphabet Inc. Google is Alphabet's largest subsidiary and is a holding company for Alphabet's Internet interests. Sundar Pichai was appointed CEO of Google, replacing Larry Page, who became the CEO of Alphabet. In 2021, the Alphabet Workers Union was founded, mainly composed of Google employees.[13] The company's rapid growth since incorporation has included products, acquisitions, and partnerships beyond Google's core search engine, (Google Search). It offers services designed for work and productivity (Google Docs, Google Sheets, and Google Slides), email (Gmail), scheduling and time management (Google Calendar), cloud storage (Google Drive), instant messaging and video chat (Google Duo, Hangouts, Google Chat, and Google Meet), language translation (Google Translate), mapping and navigation (Google Maps, Waze, Google Earth, and Street View), podcast hosting (Google Podcasts), video sharing (YouTube), blog publishing (Blogger), note-taking (Google Keep and Jamboard), and photo organizing and editing (Google Photos). The company leads the development of the Android mobile operating system, the Google Chrome web browser, and Chrome OS, a lightweight operating system based on the Chrome browser. Google has moved increasingly into hardware; from 2010 to 2015, it partnered with major electronics manufacturers in the production of its Google Nexus devices, and it released multiple hardware products in October 2016, including the Google Pixel line of smartphones, Google Home smart speaker, Google Wifi mesh wireless router, and Google Daydream virtual reality headset. Google has also experimented with becoming an Internet carrier (Google Fiber, Google Fi, and Google Station).[14] On August 8, 2017, Google fired employee James Damore after he distributed a memo throughout the company which argued that bias and Googles Ideological Echo Chamber clouded their thinking about diversity and inclusion, and that it is also biological factors, not discrimination alone, that cause the average woman to be less interested than men in technical positions.[91] Google CEO Sundar Pichai accused Damore in violating company policy by advancing harmful gender stereotypes in our workplace, and he was fired on the same day.[92][93][94] New York Times columnist David Brooks argued Pichai had mishandled the case, and called for his resignation.[95][96].Between 2018 and 2019, tensions between the company's leadership and its workers escalated as staff protested company decisions on internal sexual harassment, Dragonfly, a censored Chinese search engine, and Project Maven, a military drone artificial intelligence, which had been seen as areas of revenue growth for the company.[97][98] On October 25, 2018, The New York Times published the exposé, How Google Protected Andy Rubin, the ‘Father of Android. The company subsequently announced that 48 employees have been fired over the last two years for sexual misconduct.[99] On November 1, 2018, more than 20,000 Google employees and contractors staged a global walk-out to protest the company's handling of sexual harassment complaints.[100][101] Later in 2019, some workers accused the company of retaliating against internal activists.[98]On March 19, 2019, Google announced that it would enter the video game market, launching a cloud gaming platform called Google Stadia.[102] On June 3, 2019, the United States Department of Justice reported that it would investigate Google for antitrust violations.[103] This led to the filing of an antitrust lawsuit in October 2020, on the grounds the company had abused a monopoly position in the search and search advertising markets.[104]In December 2019, former PayPal chief operating officer Bill Ready became Google's new commerce chief. Ready's role will not be directly involved with Google Pay.[105]In April 2020, due to the COVID-19 pandemic, Google announced several cost-cutting measures. Such measures included slowing down hiring for the remainder of 2020, except for a small number of strategic areas, recalibrating the focus and pace of investments in areas like data centers and machines, and non-business essential marketing and travel.[106]The 2020 Google services outages disrupted Google services: one in August that affected Google Drive among others, another in November affecting YouTube, and a third in December affecting the entire suite of Google applications. All three outages were resolved within hours.[107][108][109].";
    
    char *readBuff = (char *) malloc(BLOCK_SIZE);
    write_data(fd, 0, buff, 6000);
    read_data(fd,0,readBuff,BLOCK_SIZE);

    FILE * f = fopen("newfile.txt","w");
    for (i = 0; i < 4096; i++) fprintf(f,"%c ",readBuff[i]);
    fclose(f);
    printf("\n");
    
    free(readBuff);
    free(block);
    
    closeDisk(fd);

    free(fd);

    return 0;
}