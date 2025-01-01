/* 
	:: C D I 4 D C ::
	
	By SiZiOUS, http://sbibuilder.shorturl.com/
	13 april 2007, v0.3b
	
	File : 	CDIAUDIO.C
	Desc : 	Contains all Audio/Data image related.
			
			Sorry comments are in french. If you need some help you can contact me at sizious[at]dc-france.com.
*/

#include "cdibuild.h"
#include "tools.h"

// ecrire la piste audio
int write_cdi_audio_track(FILE *cdi, uint64_t cdi_audio_track_total_size) {
	if (cdi == NULL) return 1;
	
	// ecrire premier secteur audio
	write_null_block(cdi, cdi_start_file_header);
	
	// ecrire la piste audio elle m�me
	write_null_block(cdi, cdi_audio_track_total_size);
	
	return 0;
}

// ecrire le secteur 2 (donn�es) du header CDI. il contient le nombre de secteurs de donn�es
void write_cdi_head_data_sector(FILE* cdi, long data_sector_count, uint64_t cdda_sector_count) {
    uint8_t *buf = (uint8_t*) malloc(sector2_size);
	fill_buffer(buf, sector2_size, sector2_entries, sector2);
	
	uint64_t data_sector_start_lba = cdda_sector_count + MSINFO_OFFSET_BASE;

	buf[0x06] = data_sector_count;
	buf[0x07] = data_sector_count >> 8;
	buf[0x08] = data_sector_count >> 16;
	buf[0x09] = data_sector_count >> 31;
	
	buf[0x24] = data_sector_count + 150;
	buf[0x25] = (data_sector_count + 150) >> 8;
	buf[0x26] = (data_sector_count + 150) >> 16;
	buf[0x27] = (data_sector_count + 150) >> 31;
	
	buf[0x41] = data_sector_count + 150;
	buf[0x42] = (data_sector_count + 150) >> 8;
	buf[0x43] = (data_sector_count + 150) >> 16;
	buf[0x44] = (data_sector_count + 150) >> 31;

	buf[0x20] = data_sector_start_lba;
	buf[0x21] = data_sector_start_lba >> 8;
	buf[0x22] = data_sector_start_lba >> 16;
	buf[0x23] = data_sector_start_lba >> 31;

	buf[0xB8] = data_sector_start_lba;
	buf[0xB9] = data_sector_start_lba >> 8;
	buf[0xBA] = data_sector_start_lba >> 16;
	buf[0xBB] = data_sector_start_lba >> 31;

	fwrite(buf, sector2_size, 1, cdi);
	free(buf);
}

void write_cdi_head_audio_sector(FILE* cdi, uint64_t cdda_sector_count) {
	uint8_t* buf = (uint8_t*)malloc(sector1_size);
	fill_buffer(buf, sector1_size, sector1_entries, sector1);

	buf[0x06] = cdda_sector_count;
	buf[0x07] = cdda_sector_count >> 8;
	buf[0x08] = cdda_sector_count >> 16;
	buf[0x09] = cdda_sector_count >> 31;

	buf[0x24] = cdda_sector_count + 150;
	buf[0x25] = (cdda_sector_count + 150) >> 8;
	buf[0x26] = (cdda_sector_count + 150) >> 16;
	buf[0x27] = (cdda_sector_count + 150) >> 31;

	buf[0x41] = cdda_sector_count + 150;
	buf[0x42] = (cdda_sector_count + 150) >> 8;
	buf[0x43] = (cdda_sector_count + 150) >> 16;
	buf[0x44] = (cdda_sector_count + 150) >> 31;

	fwrite(buf, sector1_size, 1, cdi);
	free(buf);
}

// ecrire l'header � la fin du fichier. Cette fonction appelle toutes les autres situ�es ci dessus !
void write_audio_cdi_header(FILE *cdi, char* cdiname, char* volume_name, long data_sector_count, uint32_t total_cdi_space_used, uint64_t cdda_sector_count) {
	
	struct cdi_header head;
    uint32_t cdi_end_image_tracks;
	
	cdi_end_image_tracks = ftell(cdi); // emplacement de l'header
	
	// en t�te
	head.track_count = 0x02;
	head.first_track_num = 0x01;
	head.padding = 0x00000000;
	
#ifdef WIN32
	get_full_filename(cdiname, cdiname); // on remplace l'ancien nom relatif par le nom absolu
#endif

	// ecrire le d�but de l'header
	fwrite(&head, sizeof(head), 1, cdi);
	
	// sector 1 (audio)
	write_cdi_header_start(cdi, cdiname);
	write_cdi_head_audio_sector(cdi, cdda_sector_count);
	
	// sector 2 (data)
	write_cdi_header_start(cdi, cdiname);
	write_cdi_head_data_sector(cdi, data_sector_count, cdda_sector_count);
	
	// sector 3 (fin de l'header)
	write_cdi_header_start(cdi, cdiname);
	write_cdi_head_end(cdi, volume_name, total_cdi_space_used, cdi_end_image_tracks);    
}
