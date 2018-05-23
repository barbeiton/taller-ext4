#include <iostream>
#include <assert.h>

#include "hdd.h"
#include "mbr.h"
#include "pentry.h"
#include "ext2fs.h"


using namespace std;

void test_file_system(Ext2FS * fs){
	cout << "=== Filesystem Superblock ===" << endl;
	cout << *(fs->superblock()) << endl;

	//Verifico que la informacion de la cantidad de bloques sea la esperada
	assert(fs->superblock()->blocks_count == 102400);
}

void test_hdd(HDD * hdd){
	unsigned char buffer[SECTOR_SIZE];
	hdd->read(0, buffer);
	MBR mbr(buffer);
	const PartitionEntry & pentry = mbr[1];
	cout << "=== Partition Data ===" << endl;
	cout << pentry << endl << endl;

	//Verifico que la LBA empiece donde se espera
	assert(pentry.start_lba() == 4096);
}

void test_block_groups(Ext2FS * fs){
	cout << "=== Block Groups Data ===" << endl;
	unsigned int block_groups = fs->block_groups();
	for(unsigned int i = 0; i < block_groups; i++)
	{
		cout << *(fs->block_group(i)) << endl;
	}
	Ext2FSBlockGroupDescriptor block_group = *(fs->block_group(1));

	//Verifico que el block group 1 tenga la información correcta
	assert(block_group.block_bitmap == 8195);
}

int main(int argc, char ** argv)
{
	HDD hdd(argv[1]);

	//Esto lo pueden comentar. Es sólo para ver si descomprimieron bien la imagen de disco
	test_hdd(&hdd);

	Ext2FS * fs = new Ext2FS(hdd, 1);

	//Esto lo pueden comentar. Es sólo para ver si el FS está bien
	test_file_system(fs);

	//Esto lo pueden comentar. Es sólo para ver si el FS está bien
	test_block_groups(fs);

	//TODO: Completar el código del último ejercicio acá
    cout << "Abriendo archivo" << endl;
    fd_t file = fs->open("/grupos/g15/nota.txt", "aaaaaaa!");
    if (file == -1) {
        cout << "Error abriendo archivo" << endl;
        exit(-1);
    }
    cout << "Archivo abierto" << endl;

    cout << "Empezando seek" << endl;
    fs->seek(file, 14000);
    cout << "Finalizado seek" << endl;


    cout << "Leyendo el archivo" << endl;
    unsigned char *out = new unsigned char[50];
    out[0] = 0;
    fs->read(file, out, 50);
    if (out[0] == 0)
        cout << "No se leyo nada" << endl;
    else
        cout << out << endl;
    
    cout << "Terminado de leer archivo" << endl;
    fs->close(file);
    
    delete[] out;
	return 0;
}
