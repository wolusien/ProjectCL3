#include "manip.h"

error free_block(disk_id *id, int numblock, int volume){
  error e;
  if(id!=NULL){
    if(0<=volume && volume<id->nbPart){
      Part here=id->tabPart[volume];
      if(here.file_table_size!=0){
	if(numblock>1+here.file_table_size && numblock<here.taille){
	  int prec=here.first_free_block;                   //on va chercher le dernier block du chainage pour le racorder.
	  if(prec!=0){ // si first_free_block vaut 0 alors il n'y a plus de block libres.
	    block b;
	    read_block(*id, &b, prec+here.num_first_block);
	    int suiv;
	    readint_block(&b,&suiv, 1020); 
	    
	    while(prec!=suiv){
	      if (prec==numblock){
		e.errnb=-1;
		printf("free_block : block déja libre \n");
		return e;
	      }
	      prec=suiv;
	      read_block(*id, &b, prec+here.num_first_block);
	      readint_block(&b,&suiv, 1020); 
	    }                                                // on a trouvé le dernier block du chainage
	    fill_block(&b, numblock, 1020);
	    write_block((*id), b, prec+here.num_first_block);
	    write_block(*id, b, numblock+here.num_first_block);
	    here.free_block_count+=1;
	    block first;
	    read_block(*id,&first,here.num_first_block);
	    fill_block(&first, here.free_block_count,12);
	    write_block((*id),first,here.num_first_block);
	  }else{  // cas ou il n'y avait plus de blocks libres; on doit réinitialiser le chainage. 
	    block libre;
	    int i;
	    for(i=0; i<1020; i++){
	      libre.buff[i]='\0';
	    }
	    fill_block(&libre, numblock, 1020);
	    write_block(*id, libre, numblock+here.num_first_block);
	    here.free_block_count+=1;
	    here.first_free_block=numblock;
	    block first;
	    read_block(*id,&first,here.num_first_block);
	    fill_block(&first, here.free_block_count,12);
	    fill_block(&first, here.first_free_block,16);
	    write_block((*id),first,here.num_first_block);
	  }
	  e.errnb=0;
	  return e;
	}else{
	  e.errnb=-1;
	  printf("free_block : wrong numblock : %d \n", numblock);
	  return e;
	}
      }else{
	e.errnb=-1;
	printf("free_block : volume hasn't been format  \n");
	return e;
      }
    }else{
      e.errnb=-1;
      printf("free_block : no volume  : %d \n", volume);
      return e;
    }
  }else{
    e.errnb=-1;
    printf("free_block : id NULL \n");
    return e;
  }
}


error add_free_file(disk_id id,int volume,int file){
  error e;
  block filetable;
  block description_block;
  if(volume<id.nbPart){
    if(file != 0){
      if(id.tabPart[volume].free_file_count != id.tabPart[volume].max_file_count){
	int position = id.tabPart[volume].num_first_block;
	if(id.tabPart[volume].first_free_file==0){
	  read_block(id,&description_block,int_to_little(position));
	  fill_block(&description_block,file,28);
	  fill_block(&description_block,1,24);
	  id.tabPart[volume].free_file_count=1;
	  id.tabPart[volume].first_free_file=file;
	  write_block(id,description_block,int_to_little(position));
	  position=position+(file)/16+1;
	  read_block(id,&filetable,int_to_little(position));
	  id.tabPart[volume].first_free_file = file;
	  fill_block(&filetable,file,(((file*64)%1024)-4));
	  write_block(id,filetable,position);		 
	  e.errnb = 0;
	  return e;
	}
	else{
	  int next;
	  int current = id.tabPart[volume].first_free_file;
	  int pos_bloc = ((current/16)+1);
	  read_block(id,&filetable,int_to_little(pos_bloc+position));
	  readint_block(&filetable,&next,(((current*64)%1024)-4));
	  while(next==current){
	    if(current !=file){
	      current = next;
	      pos_bloc = ((next/16)+1);
	      read_block(id,&filetable,int_to_little(pos_bloc+position));
	      readint_block(&filetable,&next,(((current*64)%1024)-4));
	    }
	    else{
	      e.errnb = -1;
	      printf("this file are already free");
	      return e;
	    }
	  }
	  fill_block(&filetable,current,(((file*64)%1024)-4));
	  write_block(id,filetable,position);
	  pos_bloc = ((file/16)+1);
	  read_block(id,&filetable,int_to_little(pos_bloc+position));
	  fill_block(&filetable,file,(((file*64)%1024)-4));
	  write_block(id,filetable,int_to_little(pos_bloc+position));
	  read_block(id,&description_block,int_to_little(position));
	  id.tabPart[volume].free_file_count++;
	  fill_block(&description_block,id.tabPart[volume].free_file_count,24);
	  e.errnb = 0;
	}
      }
      else{
	e.errnb = -1;
	printf("all the files in this volume are already free");
	return e;
      }
    }
    else{
      e.errnb = -1;
      printf("vous ne pouvez pas liberez le repertoire racine");
      return e;
    }
  }
  else{
    printf("volume %d doesn't exist on %s",volume,id.name);
    e.errnb = -1;
    return e;
  }
  return e;
}


error remove_free_file(disk_id id,int volume,int file){
  error e;
  block filetable;
  block description_block;
  if(volume<id.nbPart){
    int position = id.tabPart[volume].num_first_block;
    if(id.tabPart[volume].free_file_count != 0){
      if(id.tabPart[volume].free_file_count !=1){
	int current =  id.tabPart[volume].first_free_block;
	int past;
	int pos_bloc=((current/16)+1);
	int tour=0;
	while(current !=file && current != past && tour<id.tabPart[volume].free_file_count){
	  past=current;
	  read_block(id,&filetable,int_to_little(pos_bloc+position));
	  readint_block(&filetable,&current,(((past*64)%1024)-4));
	  pos_bloc=((current/16)+1);
	  tour++;
	}
	if(current==file){
	  int next;
	  readint_block(&filetable,&next,(((current*64)%1024)-4));
	  if(past==0){
	    fill_block(&description_block,next,28);
	    write_block(id,description_block,int_to_little(position));
	    id.tabPart[volume].first_free_file=next;
	  }
	  else if(next==current){
	    pos_bloc=((past/16)+1);
	    read_block(id,&filetable,int_to_little(pos_bloc+position));
	    fill_block(&filetable,past,(((past*64)%1024)-4));
	    write_block(id,filetable,(pos_bloc+position));
	  }
	  else{
	    pos_bloc=((past/16)+1);
	    read_block(id,&filetable,int_to_little(pos_bloc+position));
            fill_block(&filetable,next,(((past*64)%1024)-4));
            write_block(id,filetable,(pos_bloc+position));
	  }
	  id.tabPart[volume].free_file_count--;
	  fill_block(&filetable,id.tabPart[volume].free_file_count,24);
	  e.errnb=0;
	}
	else{
	  e.errnb = -1;
	  printf("file number %d isn't free",file);
	  return e;
	}
      }
      else{
	read_block(id,&description_block,int_to_little(position));
	fill_block(&description_block,0,28);
	fill_block(&description_block,0,24);
	id.tabPart[volume].free_file_count=0;
	id.tabPart[volume].first_free_file=0;
	write_block(id,description_block,int_to_little(position));
	e.errnb = 0;
      }
    }
    else{
      e.errnb = -1;
      printf("there is no free files");
      return e;
    }
  }
  else{
    e.errnb =-1;
    printf("volume %d doesn't exist on %s",volume,id.name);
    return e;
  }
  return e;
}

/*name: name of the disk wich will be used
  id_f : number file on file Table  
  id_block : id of the block
  id_part : id of the partition in tabPart*/




/*
name: name of the disk wich will be used
id_f : number file on file Table  
id_block : id of the block
id_part : id of the partition in tabPart
*/
/*
error add_file_block(disk_id disk,int id_part,int id_f, int id_block){
  error e;
  error e1;
  error e2;
  int pBlock = 0;
  int i;
  //En attente de mettre un bloc dans la liste des blocs libres
  // supprimer un bloc de la liste des blocs libres
  //the block has been delete successfully from the free blocks list
  if(e1.errnb != -1){
    Part p = (*disk).tabPart[id_part];
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
    e2 = read_block((*disk),(&fblock),int_to_little(nfdisk));
    if(e2.errnb != 1){
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
      }
    }else{
      fprintf(stderr,"Impossible to access to the information of the file.\nVerify the id_part and id_f.\n");
      e.errnb = -1;
    }
  }else{
    fprintf(stderr,"The block that you want to add is not a free block\n");
    e.errnb = -1;
  }
  return e;
}
*/
