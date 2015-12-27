#include "manip.h"


/*name: name of the disk wich will be used
  id_f : number file on file Table  
  id_block : id of the block
  id_part : id of the partition in taillePart*/

/*
error add_file_block(char* name,int id_part,int id_f, int id_block){
  error e;
  error e1;
  error e2;
  error e3;
  int pBlock = 0;
  int i;

  
  if(e.errnb != -1){//the block has been delete successfully from the free blocks list
    disk_id* disk = malloc(sizeof(disk_id));
    e2 = start_disk(name,disk);
    if(e2.errnb != -1){
      Part p = (*disk).taillePart[id_part];
      //fblock contains informations about the files (including the file that we want)(tfs_size,type ...)
      block fblock;
      //end (bytes) of file f on file table
      int fbytes = 64*(id_f);
      printf("Val of fbytes %d\n",fbytes);
      int nfdisk;
      int nfdisk1 = p.num_first_block + (fbytes/1024);
      printf("Val of nfdisk1 %d\n",nfdisk1);
      int nfdisk2 = nfdisk1;
      if(fbytes%1024>0){
        nfdisk2 += 1;
      }
      if(nfdisk1 != nfdisk2){
        nfdisk = nfdisk2;
      }else{
        nfdisk = nfdisk1;
      }
      printf("Val of nfdisk %d\n", nfdisk);
      e3 = read_block((*disk),(&fblock),int_to_little(nfdisk));
      if(e3.errnb != 1){
        //fb1 place of id_f on the block which we read
        int fb1= (id_f%16);
        fb1 = ((fb1-1) * 64) ;
        uint32_t size_file;
        unsigned char* usize = (unsigned char*)(&size_file);
        for(i=0; i<4; i++){
          usize[i] = fblock.buff[i+fb1];
          print("value of i+fb1 %d\n",i+fb1);
        }
        int sizef = (little_to_int(size_file))/1024;
        printf("val of sizef %d number of block of the file\n",sizef);
        if(sizef<10){
          //Case where no indirect blocks are used
          //lfblock last block fill by the file data
          uint32_t lfblock;
          unsigned char* lfb = (unsigned char*)(&lfblock);
          for(i=0; i<4; i++){
            lfb[i] = fblock.buff[fb1+i+12+((size-1)*4)];
            printf("Value of fb1+i+12+((size-1)*4) %d\n",fb1+i+12+((size-1)*4));
          }
          //lf block's number which is last block containing data and we must change the 4 last bytes, used later
          int lf = little_to_int(lfblock);
          printf("Value of last block data file lf %d\n", lf);
          //add the id_block for the file on list of 10 blocks
          uint32_t uid_block = int_to_little(id_block);
          unsigned char* uidb = (unsigned char*)(&uid_block);
          for(i=0; i<4; i++){
            fblock.buff[fb1+i+12+(size*4)] = uidb[i];
            printf("Value of fb1+i+12+(size*4) %d\n", fb1+i+12+(size*4));
          }
          write_block((*disk),fblock,int_to_little(nfblock_disk));

          //Change the last 4 bytes of lbblock
          //lblock block identify by lf
          block lblock;
          uint32_t ulblock = int_to_little(p.num_first_block +(lf-1));
          read_block((*disk),(&lblock),ulblock);	
          //Get 4 last bytes of lblock wich will be write on block identify by id_block
					uint32_t 4lbytes;
					unsigned char* 4l = (unsigned char*)(&4lbytes);
					for(i=0; i<4; i++){
							4l[i] = lblock.buff[i+1020];
					}
					//Change 4 last byte pointing now to id_block
					for(i=0: i<4; i++){
						lblock.buff[i+1020]= uidb[i];	
					}
					write_block((*disk), lblock,ulblock);
					//Creation of a new block that will copy at place id_block
					block b;
					b.id = id_block;
					//Change the last 4 bytes of the block in order we still have a list
					for(i=0; i<4; i++){
							b.buff[i+1020] = 4l[i];
					}
					write_block((*disk),b,int_to_little(p.num_first_block + (id_block-1)));
					stop_disk((*disk);
					
          
        }
      }else{
        fprintf(stderr,"Impossible to access to the information of the file.\nVerify the id_part and id_f.\n");
        e.errnb = -1;
      }
    }else{
      fprintf(stderr,"Impossible to start_disk.\nVerify the name of the disk.\n");
      e.errnb = -1;
    }
  }else{
    fprintf(stderr,"The block that you want to add is not a free block\n");
    e.errnb = -1;
  }
  return e;
}
*/
