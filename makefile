#################   MACRO definition  ################ 

CC = gcc

OBJFILES = blockio.o \
           open_file.o \
           read_file.o \
           write_file.o \
           close_file.o \
           create_file.o \
           delete_file.o \
           read_dir.o \
           get_type.o \
           get_size.o \
           initialize.o \
           super_block.o \
           i_node.o 

TESTCMD = sfstest


##############  Linking the Object files ----> "sfstest" ########## 

$(TESTCMD): $(TESTCMD).o sfslib.a
	$(CC) $(TESTCMD).o sfslib.a -o $(TESTCMD)

##############  Compiling the sfstest.c  ##################

$(TESTCMD).o: $(TESTCMD).c
	$(CC) -c -g $(TESTCMD).c

##############  Creating the library "sfslib.a  ##########

sfslib.a: $(OBJFILES)
	ar rc sfslib.a $(OBJFILES)
	ranlib sfslib.a

##############  Compiling the source codes   ##############

blockio.o: blockio.c blockio.h
	$(CC) -c -g blockio.c

open_file.o: open_file.c globaldata.h 
	$(CC) -c -g open_file.c

read_file.o: read_file.c globaldata.h
	$(CC) -c -g read_file.c

write_file.o: write_file.c globaldata.h
	$(CC) -c -g write_file.c

close_file.o: close_file.c globaldata.h
	$(CC) -c -g close_file.c

create_file.o: create_file.c globaldata.h
	$(CC) -c -g create_file.c

delete_file.o: delete_file.c globaldata.h
	$(CC) -c -g delete_file.c

read_dir.o: read_dir.c globaldata.h
	$(CC) -c -g read_dir.c

get_type.o: get_type.c globaldata.h
	$(CC) -c -g get_type.c

get_size.o: get_size.c globaldata.h
	$(CC) -c -g get_size.c

initialize.o: initialize.c globaldata.h
	$(CC) -c -g initialize.c

super_block.o: super_block.c globaldata.h
	$(CC) -c -g super_block.c
	
I_node.o: i_node.c globaldata.h
	$(CC) -c -g i_node.c


