#pragma once

#define PRINTER_PAGE_SIZE          uint16_t
#define PRINTER_A5_PAGE  		   25
#define PRINTER_A4_PAGE   		   26
#define PRINTER_A3_PAGE   		   27
#define PRINTER_MONARCH            80
#define PRINTER_COM_10             81
#define PRINTER_INTERNATIONAL_DL   90
#define PRINTER_INTERNATIONAL_C5   91

#define PRINTER_MEDIA_TYPE     char*
#define PRINTER_PLAIN_PAPER    "6WdPlain"
#define PRINTER_REGULAR_PAPER  "8WdRegular"

// file:///C:/Users/PC/Desktop/pcl.pdf
void ParallelPrinterEnableAutomaticTextWrap()
{
	ParallelSendByte(27);
	ParallelSendByte('&');
	ParallelSendByte('s');
	ParallelSendByte('0');
	ParallelSendByte('C');
}
void ParallelPrinterDisableAutomaticTextWrap()
{
	ParallelSendByte(27);
	ParallelSendByte('&');
	ParallelSendByte('s');
	ParallelSendByte('1');
	ParallelSendByte('C');
}
void ParallelPrinterDisableEscapeSequences()
{
	ParallelSendByte(27);
	ParallelSendByte('Y');
}
void ParallelPrinterEnableEscapeSequences()
{
	ParallelSendByte(27);
	ParallelSendByte('Z');
}
void ParallelPrinterPrintTest()
{
	ParallelSendByte(27);
	ParallelSendByte('z');
}
void ParallelPrinterPrintAndSelectPageSize(PRINTER_PAGE_SIZE pageSize)
{
	ParallelSendByte(27);
	ParallelSendByte('&');
	ParallelSendByte('l');
	ParallelSendByte(pageSize & 0xff);
	ParallelSendByte(pageSize >> 8);
	ParallelSendByte('A');
}
void ParallelPrinterSelectMediaType(PRINTER_MEDIA_TYPE mediaType)
{
	ParallelSendByte(27);
	ParallelSendByte('&');
	ParallelSendByte('n');
	for(uint8_t i = 0; mediaType[i] != '\0'; i++)
		ParallelSendByte(mediaType[i]);
}