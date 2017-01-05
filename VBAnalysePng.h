#pragma once
#include "pngitfc.h"

#define PNG_BYTES_TO_CHECK 4 

class VBAnalysePng
{
public:
	VBAnalysePng();
	~VBAnalysePng();

public:
	void Build( VSPngOutPut & output ,const VSFileInput & input)
	{
		/*const char * buf = input.data.pData;
		const unsigned len = input.data.len;

		unsigned char png32Flag[] = { 0x89, 0x50, 0x4e, 0x47, 0xd, 0xa, 0x1a, 0xa };

		unsigned offset = 0;

		for (;offset <8;offset++)
		{
			if (buf[offset]!= png32Flag[offset])
			{
				output.flag = NOTPNG32;
				return;
			}
		}
		offset += 8;
		memcpy(&output.size.x, &buf[offset], sizeof(int));
		offset += sizeof(int);
		memcpy(&output.size.y, &buf[offset], sizeof(int));
		offset += 9;
		
		while (offset < len)

		{

			unsigned int dataLen = 0; //���ݳ���


			memcpy(&dataLen, &buf[offset], sizeof(int));
			offset += sizeof(int);
			/ *for (unsigned int i = offset + 4; offset < i; offset++)

			{

				dataLen += buf[offset] * pow((double)256, (int)(i - offset - 1));

			}* /

			//�����ͼ�����ݣ� ��д����һ���ļ���

			
			offset += (dataLen + 4); //�������ݺ�CRCУ����

		}*/

		const char * filepath = input.name.pData;
		FILE *fp;
		png_structp png_ptr;
		png_infop info_ptr;
		png_bytep* row_pointers;
		char buf[PNG_BYTES_TO_CHECK];
		int  temp;

		fopen_s(&fp,filepath, "rb");
		if (fp == NULL) {
			output.flag = OPENERROR;
			return /* ����ֵ */;
		}

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		info_ptr = png_create_info_struct(png_ptr);

		setjmp(png_jmpbuf(png_ptr));
		/* ��ȡPNG_BYTES_TO_CHECK���ֽڵ����� */
		temp = fread(buf, 1, PNG_BYTES_TO_CHECK, fp);
		/* �����������ݲ�û��PNG_BYTES_TO_CHECK���ֽ� */
		if (temp < PNG_BYTES_TO_CHECK) {
			fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			output.flag = NOTPNG32;
			return /* ����ֵ */;
		}
		/* ��������Ƿ�ΪPNG��ǩ�� */
		temp = png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
		/* �������PNG��ǩ������˵�����ļ�����PNG�ļ� */
		if (temp != 0) {
			fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			output.flag = NOTPNG32;
			return /* ����ֵ */;
		}

		/* ��λ�ļ�ָ�� */
		rewind(fp);
		/* ��ʼ���ļ� */
		png_init_io(png_ptr, fp);
		/* ��ȡPNGͼƬ��Ϣ */
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
		/* ��ȡͼ���ɫ������ */
		m_pngColorType = png_get_color_type(png_ptr, info_ptr);
		/* ��ȡͼ��Ŀ�� */
		m_pngWidth = png_get_image_width(png_ptr, info_ptr);
		m_pngHeight = png_get_image_height(png_ptr, info_ptr);
		/* ��ȡͼ����������������ݣ�row_pointers��߾���rgba���� */
		row_pointers = png_get_rows(png_ptr, info_ptr);


		output.size.x = m_pngWidth;
		output.size.y = m_pngHeight;
		m_pngRowInfo.resize(m_pngHeight*m_pngWidth);
		int x = 0, y = 0 ,pos = 0;
		switch (m_pngColorType) {
		case PNG_COLOR_TYPE_RGB_ALPHA:
			for (y = 0; y < m_pngHeight; ++y) {
				for (x = 0; x < m_pngWidth * 4; ) {
					
					/* ������RGBA���ݣ���Ҫ�Լ�������룬����RGBA���� */
					m_pngRowInfo[pos].r= row_pointers[y][x++]; // red  
					m_pngRowInfo[pos].g = row_pointers[y][x++]; // green  
					m_pngRowInfo[pos].b = row_pointers[y][x++]; // blue  
					m_pngRowInfo[pos].a = row_pointers[y][x++]; // alpha  
					pos++;
				}
			}
			break;

		case PNG_COLOR_TYPE_RGB:
			for (y = 0; y < m_pngHeight; ++y) {
				for (x = 0; x < m_pngWidth * 3; ) {
					m_pngRowInfo[pos].r = row_pointers[y][x++]; // red  
					m_pngRowInfo[pos].g = row_pointers[y][x++]; // green  
					m_pngRowInfo[pos].b = row_pointers[y][x++]; // blue  
					m_pngRowInfo[pos].a = 1;
					pos++;
				}
			}
			break;
			/* ����ɫ�����͵�ͼ��Ͳ����� */
		default:
			fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			output.flag = OTHER;
			return /* ����ֵ */;
		}

		output.flag = NORMAL;
		output.info.pData = &m_pngRowInfo[0];
		output.info.len = m_pngHeight*m_pngWidth;
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	}
private:
	bool IsBig_endian()
	{
		union MyUnion
		{
			int a;
			char b;
		}c;
		c.a = 1;
		if (c.b == 1)
			return false;
		else
			return true;

	}

private:

	int m_pngWidth;
	int m_pngHeight;
	int m_pngColorType;
	std::vector<VSColor> m_pngRowInfo;
};

VBAnalysePng::VBAnalysePng()
{
}

VBAnalysePng::~VBAnalysePng()
{
}
