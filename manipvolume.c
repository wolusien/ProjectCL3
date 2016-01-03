#include "manipvolume.h"

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
	  fprintf(stderr,"free_block : wrong numblock : %d \n", numblock);
	  return e;
	}
      }else{
	e.errnb=-1;
	fprintf(stderr,"free_block : volume hasn't been format  \n");
	return e;
      }
    }else{
      e.errnb=-1;
      fprintf(stderr,"free_block : no volume  : %d \n", volume);
      return e;
    }
  }else{
    e.errnb=-1;
    fprintf(stderr,"free_block : id NULL \n");
    return e;
  }
}

error use_block(disk_id *id, int numblock, int volume){
  error e;
  if(id!=NULL){
    if(0<=volume && volume<id->nbPart){
      Part here=id->tabPart[volume];
      if(here.file_table_size!=0){
	if(numblock>1+here.file_table_size && numblock<here.taille){
	  int prec=here.first_free_block;                   //on va chercher le block "numblock" pour le dechainer.
	  if(prec!=0){
	    block p;
	    read_block(*id, &p, prec+here.num_first_block);
	    int suiv;
	    readint_block(&p,&suiv, 1020); 
	    block s;
	    block first;
	    read_block(*id,&first,here.num_first_block);
	    if( suiv==numblock){                                   //Cas ou numbloc est le premier bloc libre
	      if(suiv==prec){                                    //Cas ou c'est le seul bloc libre
		fill_block(&first, 0,12);
		fill_block(&first, 0,16);
		write_block((*id),first,here.num_first_block);
		here.free_block_count=0;
		here.first_free_block=0;
	      }else{
		fill_block(&first, (here.free_block_count-1),12);
		fill_block(&first, suiv,16);
		write_block((*id),first,here.num_first_block);
		here.free_block_count-=1;
		here.first_free_block=suiv;
	      }
	      e.errnb=0;
	      return e;
	    }else{                                             //sinon on cherche le prédécesseur de numbloc dans le chainage.
	      while(prec!=suiv){                                    
		if (suiv==numblock){      //on a trouvé numblock  	  
		  int a;
		  read_block(*id, &s, suiv+here.num_first_block);
		  readint_block(&s,&a, 1020); 
		  if(a==suiv){                   //cas ou le block "numblock" est le dernier du chainage.
		    fill_block(&s,prec,1020);
		  }
		  write_block((*id),s,here.num_first_block+prec);
		  fill_block(&first, here.free_block_count+1,12);
		  write_block((*id),first,here.num_first_block);
		  here.free_block_count-=1;
		  e.errnb=0;
		  return e;
		}
		prec=suiv;
		read_block(*id, &p, prec+here.num_first_block);
		readint_block(&p,&suiv, 1020); 
	      }
	      e.errnb=-1;
	      printf("use_block : this block isn't free  : %d \n", numblock);
	      return e;
	    }
	  }else{
	    e.errnb=-1;
	    printf("use_block : volume is full  : %d \n", numblock);
	    return e;
	  } 
	}else{
	  e.errnb=-1;
	  printf("use_block : wrong numblock : %d \n", numblock);
	  return e;
	}
      }else{
	e.errnb=-1;
	printf("use_block : volume hasn't been format  \n");
	return e;
      }
    }else{
      e.errnb=-1;
      printf("use_block : no volume  : %d \n", volume);
      return e;
    }
  }else{
    e.errnb=-1;
    printf("use_block : id NULL \n");
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
	      fprintf(stderr,"this file are already free");
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
      fprintf(stderr,"vous ne pouvez pas liberez le repertoire racine");
      return e;
    }
  }
  else{
    fprintf(stderr,"volume %d doesn't exist on %s",volume,id.name);
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
	  fprintf(stderr,"file number %d isn't free",file);
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
      fprintf(stderr,"there is no free files");
      return e;
    }
  }
  else{
    e.errnb =-1;
    fprintf(stderr,"volume %d doesn't exist on %s",volume,id.name);
    return e;
  }
  return e;
}



/*
disk: disk wich will be used
id_f : number file on file Table  
id_block : id of the block on partition
id_part : id of the partition in tabPart
*/
error add_file_block(disk_id* disk,int id_part,int id_f, int id_block){
  error e;
  error e1;
  int i;
  if(id_f>0 && id_block>0 && id_part>0){
    Part p = (*disk).tabPart[id_part];
    if(p.taille>id_block){
      e1 = free_block(disk,id_block,id_part);
      if(e1.errnb!=-1){
        uint32_t uid_block = int_to_little(id_block);
        unsigned char* bufid_block = (unsigned char*)(&uid_block);
        //beginning of the information of file number id_f on the file_Tab
        int deb_finfo = ((id_f-1)*64)+1;
        //num of block in partition which containning the part of file_Tab which contains id_f
        int numb_finfo = deb_finfo/1024;
        if(deb_finfo%1024>0){
          numb_finfo += 1;
        }
        numb_finfo += p.num_first_block;//matches with the description block of the partition
        printf("val of numb_finfo %d\n",numb_finfo); 
        block b_finfo;
        read_block((*disk),(&b_finfo),(int_to_little(numb_finfo)));
        //position of file f on b_finfo
        int posf;
        if(id_f%16 > 0){
          posf = (id_f%16)-1;
        }else{
          posf = 15;
        }
        uint32_t tsize;
        unsigned char* buftsize = (unsigned char*)(&tsize);
        for(i=0; i<4; i++){
          buftsize[i] = b_finfo.buff[i+(posf*64)+1];
          printf("Val of b_finfo.buff[i+(posf*64)+1] %d\n",i+(posf*64)+1);
        }
        int tfsize = little_to_int(tsize);
        //How many blocks are use by the file
        int btfsize = (tfsize/1020) + (tfsize%1020);
        if(btfsize<11){
          //Case where file is using only direct blocks (precisely it uses only btfsize)
          //we add the num of id_block at position btfsize+1 on file_Tab
          
          for(i=0; i<4; i++){
            b_finfo.buff[i+(posf*64)+12+(btfsize*4)]=bufid_block[i];
            printf("Val of i+(pos*64)+12+(btfsize*4) %d\n",i+(posf*64)+12+(btfsize*4));
          }
          
          write_block((*disk),b_finfo,(int_to_little(numb_finfo)));
          //Change the redirection block
          block lastblock_use;
          uint32_t l;
          unsigned char* bufl = (unsigned char*)(&l);
          for(i=0; i<4; i++){
            bufl[i] = b_finfo.buff[i+(posf*64)+12+((btfsize-1)*4)];
            printf("Val of i+(pos*64)+12+((btfsize-1)*4) %d\n",i+(posf*64)+12+((btfsize-1)*4)); 
          }
          int last = little_to_int(l);
          printf("val of last %d\n",last);
          //Read the last block use by the file for changing the redirection block at the end
          read_block((*disk),(&lastblock_use),(int_to_little(p.num_first_block + last)));
          for(i=0; i<4; i++){
            lastblock_use.buff[1020+i] = bufid_block[i];
          }
          write_block((*disk),lastblock_use,(int_to_little(p.num_first_block + last)));
          e.errnb = 0;
        }else if(btfsize<265){
          //get the number of block_indirect 1
          uint32_t b_ind;
          unsigned char* buf_ind = (unsigned char*)(&b_ind);
          for(i=0; i<4; i++){
            buf_ind[i] = b_finfo.buff[i+(posf*64)+52];
          }
          printf("val of b_ind %d\n",little_to_int(b_ind)); 
          int o = btfsize - 10;
          block indirect;
          int indirec = little_to_int(b_ind) + p.num_first_block;
          read_block((*disk),(&indirect),int_to_little(indirec));
          //Get last block use by file
          int lastbuse;
          uint32_t ulast;
          unsigned char* bufulast = (unsigned char*)(&ulast);
          for(i=0; i<4; i++){
            bufulast[i] = indirect.buff[i+((o-1)*4)];
            indirect.buff[i+(o*4)]=bufid_block[i];
            printf("Val of indirect.buff[i+(o*4)] %d\n",i+((o-1)*4));
          }
          write_block((*disk),indirect,int_to_little(indirec));
          lastbuse = (little_to_int(ulast)) + p.num_first_block;
          block lstb_use;
          read_block((*disk),(&lstb_use), (int_to_little(lastbuse)));
          for(i=0; i<4; i++){
            lstb_use.buff[i+1020] = bufid_block[i];
          }
          write_block((*disk),lstb_use,(int_to_little(lastbuse)));
          e.errnb = 0;
        }else if(btfsize<520){
          //get the number of block_indirect 2
          uint32_t b_ind;
          unsigned char* buf_ind = (unsigned char*)(&b_ind);
          for(i=0; i<4; i++){
            buf_ind[i] = b_finfo.buff[i+(posf*64)+56];
          }
          printf("val of b_ind %d\n",little_to_int(b_ind)); 
          int o = btfsize - 265;
          block indirect;
          int indirec = little_to_int(b_ind) + p.num_first_block;
          read_block((*disk),(&indirect),int_to_little(indirec));
          //Get last block use by file
          int lastbuse;
          uint32_t ulast;
          unsigned char* bufulast = (unsigned char*)(&ulast);
          for(i=0; i<4; i++){
            bufulast[i] = indirect.buff[i+((o-1)*4)];
            indirect.buff[i+(o*4)]=bufid_block[i];
            printf("Val of indirect.buff[i+(o*4)] %d\n",i+((o-1)*4));
          }
          write_block((*disk),indirect,int_to_little(indirec));
          lastbuse = (little_to_int(ulast)) + p.num_first_block;
          block lstb_use;
          read_block((*disk),(&lstb_use), (int_to_little(lastbuse)));
          for(i=0; i<4; i++){
            lstb_use.buff[i+1020] = bufid_block[i];
          }
          write_block((*disk),lstb_use,(int_to_little(lastbuse)));
          e.errnb = 0;
        }else{
          fprintf(stderr,"Full size for this file, can't add any block\n"); 
          e.errnb=-1;
        }
      }else{
        fprintf(stderr,"Wrong argument for the id_block\n"); 
        e.errnb=-1;
      }
    }else{
      fprintf(stderr,"Wrong argument for the id_block compare to the partition size\n"); 
      e.errnb=-1;
    }
  }else{
    fprintf(stderr,"Wrongs arguments\n"); 
    e.errnb=-1;	
  }
  return e;
}

iter decomposition(char *path){
  char *separateur= "//";
  char *token = strtok(path,separateur);
  iter current = malloc(sizeof(iter));
  current->name =token;
  token=strtok(NULL,separateur);
  iter it = current;
  while(token != NULL){
    iter next = malloc(sizeof(iter));
    next->name = token;
    next->prec = it;
    it->next = next;
    it = it->next;
    token=strtok(NULL,separateur);
  }
  return current;
}
//lol
/*
disk: disk wich will be used
id_f : number file on file Table  
id_block : id of the block for the file on file_Tab if direct 1<= id_block<=10 else id_block>=11
id_part : id of the partition in tabPart
*/
error remove_file_block(disk_id* disk,int id_part, int id_f, int id_block){
	error e;
	int i;
	if(id_part>0 && id_f>0 && id_block>0){
		Part p = (*disk).tabPart[id_part];
		//Get information on file_Tab about file
		block blockf;
		int deb_finfo = id_f * 64;
		int numb_debfinfo = deb_finfo / 1024;
		numb_debfinfo += p.num_first_block;
		if(deb_finfo%1024>0){
			numb_debfinfo += 1;	
		}
		read_block((*disk),(&blockf),(int_to_little(numb_debfinfo)));
		int posf_on_blockf;
		if((numb_debfinfo%16)>0){
			posf_on_blockf = (numb_debfinfo%16)-1;
		}else{
			posf_on_blockf = 15;	
		}
		uint32_t uid_block;//Contains the num of the block on partition
		unsigned char* bufid_block = (unsigned char*)(&uid_block);
		uint32_t uprev_block;//Contains the previous block use by the file which we must change redirection bytes 
		unsigned char* bufprev_block = (unsigned char*)(&uprev_block);
		if(id_block<=10){
			for(i=0; i<4; i++){
				bufid_block[i] = blockf.buff[i + (posf_on_blockf * 64)];
				printf("Val of 	blockf.buff[i + (posf_on_blockf * 64)] %d\n",i + (posf_on_blockf * 64));
				bufprev_block[i] = blockf.buff[i + ((posf_on_blockf-1) * 64)];
			}
			int idb = little_to_int(uid_block);
			printf("Val of idb %d\n",idb);
			int previdb = little_to_int(uprev_block);
			printf("Val of previdb %d\n",previdb);
			block prev_b;
			block b;
			read_block((*disk),(&b),int_to_little(p.num_first_block + idb));
			read_block((*disk),(&prev_b),int_to_little(p.num_first_block + previdb));
			uint32_t redir;
			unsigned char* bufredir = (unsigned char*)(&redir);
			for(i=0; i<4; i++){ 
				bufredir[i] = b.buff[i + 1020];
				prev_b.buff[i+1020] = bufredir[i];
			}
			printf("Val of redir %d\n", little_to_int(redir));
			write_block((*disk),prev_b,int_to_little(p.num_first_block + previdb));
			unsigned char c[1024-(posf_on_blockf * 64)];
			for(i = 0; i<1024-(posf_on_blockf * 64); i++){
				c[i] = 	blockf.buff[i + (posf_on_blockf * 64)];
				printf("Val of blockf.buff[i + (posf_on_blockf * 64)] %d\n",i + (posf_on_blockf * 64));
			}
			for(i=4; i < 1024-(posf_on_blockf * 64); i++){
				blockf.buff[i-4 + (posf_on_blockf * 64)] = c[i];
				printf("Val of blockf.buff[i + (posf_on_blockf * 64)] %d\n",i + (posf_on_blockf * 64));
			}
			write_block((*disk),blockf,int_to_little(numb_debfinfo));
			e.errnb = 0; 
		}else if(id_block<=266){
			block indi_first;
			uint32_t indi;
			unsigned char* bufindi = (unsigned char*)(&indi);
			for(i=0; i<4; i++){
				bufindi[i] = blockf.buff[i + (posf_on_blockf * 64) + 52];
				printf("Val of 	blockf.buff[i + (posf_on_blockf * 64) + 52] %d\n",i + (posf_on_blockf * 64) + 52);
			}
			int num_indi = little_to_int(indi);
			read_block((*disk),(& indi_first),int_to_little(num_indi + p.num_first_block));
			int o = id_block -11;
			for(i=0; i<4; i++){
				//Get the num of the block on partition
				bufid_block[i] = indi_first.buff[i+(o*4)];
				printf("Val of 	indi_first.buff[i+(o*4)] %d\n",i+(o*4));
				//Get the previous block use by the file which we must change redirection bytes
				bufprev_block[i] = indi_first.buff[i+((o-1)*4)];
			}
			int idb = little_to_int(uid_block);
			printf("Val of idb %d\n",idb);
			int previdb = little_to_int(uprev_block);
			printf("Val of previdb %d\n",previdb);
			block prev_b;
			block b;
			read_block((*disk),(&b),int_to_little(p.num_first_block + idb));
			read_block((*disk),(&prev_b),int_to_little(p.num_first_block + previdb));
			uint32_t redir;
			unsigned char* bufredir = (unsigned char*)(&redir);
			for(i=0; i<4; i++){ 
				bufredir[i] = b.buff[i + 1020];
				prev_b.buff[i+1020] = bufredir[i];
			}
			printf("Val of redir %d\n", little_to_int(redir));
			write_block((*disk),prev_b,int_to_little(p.num_first_block + previdb));
			unsigned char c[1024-(o * 4)];
			for(i = 0; i<1024-(o * 4); i++){
				c[i] = 	indi_first.buff[i + (o * 4)];
				printf("Val of indi_first.buff[i + (o * 4)] %d\n",i + (o * 4));
			}
			for(i=4; i < 1024-(o*4); i++){
				indi_first.buff[i-4 + (o*4)] = c[i];
				printf("Val of indi_first.buff[i + (o*4)] %d\n",i + (o*4));
			}
			write_block((*disk),indi_first,int_to_little(numb_debfinfo));
			e.errnb = 0; 
		}else{
			block indi_sec;
			uint32_t indi;
			unsigned char* bufindi = (unsigned char*)(&indi);
			for(i=0; i<4; i++){
				bufindi[i] = blockf.buff[i + (posf_on_blockf * 64) + 56];
				printf("Val of 	blockf.buff[i + (posf_on_blockf * 64) + 56] %d\n",i + (posf_on_blockf * 64) + 56);
			}
			int num_indi = little_to_int(indi);
			read_block((*disk),(&indi_sec),int_to_little(num_indi + p.num_first_block));
			int o = id_block -266;
			for(i=0; i<4; i++){
				//Get the num of the block on partition
				bufid_block[i] = indi_sec.buff[i+(o*4)];
				printf("Val of 	indi_sec.buff[i+(o*4)] %d\n",i+(o*4));
				//Get the previous block use by the file which we must change redirection bytes
				bufprev_block[i] = indi_sec.buff[i+((o-1)*4)];
			}
			int idb = little_to_int(uid_block);
			printf("Val of idb %d\n",idb);
			int previdb = little_to_int(uprev_block);
			printf("Val of previdb %d\n",previdb);
			block prev_b;
			block b;
			read_block((*disk),(&b),int_to_little(p.num_first_block + idb));
			read_block((*disk),(&prev_b),int_to_little(p.num_first_block + previdb));
			uint32_t redir;
			unsigned char* bufredir = (unsigned char*)(&redir);
			for(i=0; i<4; i++){ 
				bufredir[i] = b.buff[i + 1020];
				prev_b.buff[i+1020] = bufredir[i];
			}
			printf("Val of redir %d\n", little_to_int(redir));
			write_block((*disk),prev_b,int_to_little(p.num_first_block + previdb));
			unsigned char c[1024-(o * 4)];
			for(i = 0; i<1024-(o * 4); i++){
				c[i] = 	indi_sec.buff[i + (o * 4)];
				printf("Val of indi_sec.buff[i + (o * 4)] %d\n",i + (o * 4));
			}
			for(i=4; i < 1024-(o*4); i++){
				indi_sec.buff[i-4 + (o*4)] = c[i];
				printf("Val of indi_sec.buff[i + (o*4)] %d\n",i + (o*4));
			}
			write_block((*disk),indi_sec,int_to_little(numb_debfinfo));
			e.errnb = 0;
		}
		block empty;
		int idblock = little_to_int(uid_block);
		write_block((*disk),empty,int_to_little(p.num_first_block + idblock));
		error e1 = use_block(disk,idblock,id_part);
	}else{
		e.errnb = -1;
		fprintf(stderr, "Wrong arguments\n");
	}
	return e;
}

void file_tableau(int *tab[16],disk_id id,int volume){
    if(volume<id.nbPart){
        int position = id.tabPart[volume].num_first_block;
        int maxf = id.tabPart[volume].max_file_count;
        tab = malloc(sizeof(int)*maxf);
        int fts =id.tabPart[volume].file_table_size;
        int i;
        block ft;
        for(i=0;i<fts;i++){
            read_block(id,&ft,position+1+i);
            int j;
            int max;
            if(i==fts-1){
                max=maxf-16*i;
            }
            else{
                max=16;
            }
                for(j=0;j<max;j++){
                    int k;
                for(k=0;k<16;k++){
                    int nombre;
                    readint_block(&ft,&nombre,k*4);
                    tab[j+i*16][k]=nombre;
               }
           }
       } 
        
    }
    else{
        printf("le volume n'existe pas");
    }
    
}