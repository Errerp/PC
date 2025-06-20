#include "im_lib.h"


unsigned __int8* imread(char *file_name, BmpInfoHeader &im_info, BmpFileHeader &im_header, unsigned __int8* color_map)
{
	std::fstream	bmpFile;	
	unsigned __int8 *data;

	bmpFile.open(file_name, std::ios::binary | std::ios::in);

	if (!bmpFile){
		std::cout <<"IMREAD: Bmp-file is not opened." <<std::endl;
		return NULL;
	}
	
	// Read the header of file
	bmpFile.seekg(0, std::ios::beg);
	bmpFile.read((char*)&im_header, sizeof(im_header));
		
	std::cout<<"Size of bmp-file:"<<im_header.size1 + (im_header.size2<<16)<<std::endl;

	// Read file information	
	bmpFile.read((char*)&im_info, sizeof(im_info));
	bmpFile.read((char*)color_map, 1024);
	
	// Read the image
	data = new unsigned __int8[im_info.height*im_info.width];
	memset(data, 0, im_info.height*im_info.width);	//Zero array
	bmpFile.read((char*)data,im_info.height*im_info.width);


	bmpFile.close();
	return data;

}

int imwrite(char *file_name,unsigned __int8* image, BmpInfoHeader &im_info, BmpFileHeader &im_header, unsigned __int8* color_map)
{
	std::fstream	bmpFile;

	bmpFile.open(file_name, std::ios::binary | std::ios::out);
	
	if (!bmpFile){
		std::cout <<"IMWRITE: BMP-file is not opened." <<std::endl;
		return 1;
	}
	
	bmpFile.seekg(0, std::ios::beg);
	bmpFile.write((char*)&im_header, sizeof(im_header));
	
	bmpFile.write((char*)&im_info, sizeof(im_info));
	bmpFile.write((char*)color_map, 1024);

	bmpFile.seekg(0, std::ios::end);
	bmpFile.write((char*)image, im_info.height*im_info.width);
	
	bmpFile.close();
	return 0;
}
 