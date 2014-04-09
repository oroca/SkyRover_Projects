//----------------------------------------------------------------------------
//    프로그램명 	: printf 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Lb_Printf.h
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  LB_PRINTF_LOCAL

#include "Lb_Printf.h"



//-- 내부 선언
//
#define LB_PRINT_OUT_UART	0
#define LB_PRINT_OUT_STR	1



//-- 내부 변수
//
char   StrPrintMode = LB_PRINT_OUT_UART;
char *pStrPrintBuf;
int    StrPrintBufIndex = 0;


//-- 내부 함수
//
static void PrintChar(char *fmt, char c);
static void PrintDec(char *fmt, int value);
static void PrintHex(char *fmt, int value);
static void PrintString(char *fmt, char *cptr);
static int  Power(int num, int cnt);
static void SerialOutputString(const char *s);



//-- 외부 포팅 함수
//
extern void print_byte(unsigned int c);
extern char get_byte(void);



// print in hex value.
// type= 8 : print in format "ff".
// type=16 : print in format "ffff".
// type=32 : print in format "ffffffff".
typedef enum 
{
	VAR_LONG  = 32,
	VAR_SHORT = 16,
	VAR_CHAR  = 8
} VAR_TYPE;

typedef char *va_list;


#define va_start(ap, p)			(ap = (char *) (&(p)+1))
#define va_arg(ap, type)		((type *) (ap += sizeof(type)))[-1]
#define va_end(ap)




void print_byte_out(unsigned int c)
{
	if( StrPrintMode == LB_PRINT_OUT_UART )
	{
		print_byte(c);
	}
	else
	{
		pStrPrintBuf[ StrPrintBufIndex++ ] = c; 
		pStrPrintBuf[ StrPrintBufIndex   ] = 0; 
	}
}


// Write a null terminated string to the serial port.
void SerialOutputString(const char *s)
{
        while (*s != 0) 
        {
                print_byte_out(*s);
                // If \n, also do \r.
                if (*s == '\n') print_byte_out('\r');
                s++;        
        }
} // SerialOutputString.

// 문자열 s1, s2을 길이 len의 범위 이내에서 비교.
// return : 0 : equil                ret : s1 > s2                -ret : s1 < s2
int StrNCmp(char *s1, char *s2, int len){
        int i;


        for(i = 0; i < len; i++){
                if(s1[i] != s2[i])        return ((int)s1[i]) - ((int)s2[i]);
                if(s1[i] == 0)                return 0;
        }
        return 0;
} // StrNCmp.

// 문자열 s1, s2를 비교.
// return : 0 : equil                ret : s1 > s2                -ret : s1 < s2
int StrCmp(char *s1, char *s2)
{
        for (; *s1 && *s2; s1++, s2++)
        {
                if (*s1 != *s2) return ((int)(*s1) - (int)(*s2));
        }
        if (*s1 || *s2) return ((int)(*s1) - (int)(*s2));
        return 0;
}        // StrCmp.

// 역할 : 10진수 문자열 s에서 정수를 만들어 retval이 가리키는 위치에 기록.
// 매개 : s      : 변환할 문자열의 주소.
//        retval : 변환된 값이 기록될 주소.
// 반환 : return : 1 : success                0 : failure.
// 주의 :
int DecToLong(char *s, long *retval){
        long remainder;
        if (!s || !s[0]) return false;


        for (*retval=0; *s; s++){
                if (*s < '0' || *s > '9') return false;
                remainder = *s - '0';
                *retval = *retval * 10 + remainder;
        }


        return true;
}        // DecToLong.


// 역할 : printf() 중 일부를 간단하게 구현.
// 매개 : fmt : printf()와 동일하나 "%s", "%c", "%d", "%x" 사용 가능.
//              %d, %x의 경우에는 "%08x", "%8x"와 같이 나타낼 길이와 빈 공간을 0으로 채울지 선택 가능.
// 반환 : 없음.
// 주의 : 없음.
void Lb_printf(char *fmt, ...)
{
	int		i;
	va_list args;
	char	*s=fmt;
	char	format[10];        // fmt의 인자가 "%08lx"라면, "08l"를 임시로 기록.
        
	va_start(args, fmt);
	
	StrPrintMode = LB_PRINT_OUT_UART;
	
	while (*s)
	{
		if (*s=='%')
		{
			s++;
			// s에서 "%08lx"형식을 가져와 format에 기록. 나중에 출력함수에 넘겨줌.
			format[0] = '%';
			
			for (i=1; i<10;)
			{
				if (*s=='c' || *s=='d' || *s=='x' || *s=='s' || *s=='%')
				{
					format[i++] = *s;
					format[i] = '\0';
                	break;
				}
				else 
				{
					format[i++] = *s++;
				}
			}
			
			// "%s", "%c", "%d", "%x"를 찾아 출력할 함수 호출.
			switch (*s++)
			{
				case 'c' :
					PrintChar(format, va_arg(args, int));
					break;
				case 'd' :
					PrintDec(format, va_arg(args, int));
					break;
				case 'x' :
					PrintHex(format, va_arg(args, int));
					break;
				case 's' :
					PrintString(format, va_arg(args, char *));
 					break;
				case '%' :
					PrintChar("%c", '%');
					break;
            }
		}
		else 
		{
			PrintChar("%c", *s);
			s++;
		}
	}
	va_end(args);
	return;
}





/*---------------------------------------------------------------------------
     TITLE   : Lb_sprintf
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Lb_sprintf( char *pStr, char *fmt, ... )
{
	int		i;
	char	*s=fmt;
	char	format[10];        // fmt의 인자가 "%08lx"라면, "08l"를 임시로 기록.
	va_list args;
	        	
	StrPrintMode = LB_PRINT_OUT_STR;
	pStrPrintBuf = pStr;
	StrPrintBufIndex = 0;
	
	va_start(args, fmt);
	
	while (*s)
	{
		if (*s=='%')
		{
			s++;
			// s에서 "%08lx"형식을 가져와 format에 기록. 나중에 출력함수에 넘겨줌.
			format[0] = '%';
			
			for (i=1; i<10;)
			{
				if (*s=='c' || *s=='d' || *s=='x' || *s=='s' || *s=='%')
				{
					format[i++] = *s;
					format[i] = '\0';
                	break;
				}
				else 
				{
					format[i++] = *s++;
				}
			}
			
			// "%s", "%c", "%d", "%x"를 찾아 출력할 함수 호출.
			switch (*s++)
			{
				case 'c' :
					PrintChar(format, va_arg(args, int));
					break;
				case 'd' :
					PrintDec(format, va_arg(args, int));
					break;
				case 'x' :
					PrintHex(format, va_arg(args, int));
					break;
				case 's' :
					PrintString(format, va_arg(args, char *));
 					break;
				case '%' :
					PrintChar("%c", '%');
					break;
            }
		}
		else 
		{
			PrintChar("%c", *s);
			s++;
		}
	}
	
	va_end(args);
	
	return;
}





/*---------------------------------------------------------------------------
     TITLE   : Lb_vsprintf
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Lb_vsprintf( char *pStr, char *fmt, Lb_va_list args )
{
	int		i;
	char	*s=fmt;
	char	format[10];        // fmt의 인자가 "%08lx"라면, "08l"를 임시로 기록.
	        	
	StrPrintMode = LB_PRINT_OUT_STR;
	pStrPrintBuf = pStr;
	StrPrintBufIndex = 0;
	
	
	while (*s)
	{
		if (*s=='%')
		{
			s++;
			// s에서 "%08lx"형식을 가져와 format에 기록. 나중에 출력함수에 넘겨줌.
			format[0] = '%';
			
			for (i=1; i<10;)
			{
				if (*s=='c' || *s=='d' || *s=='x' || *s=='s' || *s=='%')
				{
					format[i++] = *s;
					format[i] = '\0';
                	break;
				}
				else 
				{
					format[i++] = *s++;
				}
			}
			
			// "%s", "%c", "%d", "%x"를 찾아 출력할 함수 호출.
			switch (*s++)
			{
				case 'c' :
					PrintChar(format, va_arg(args, int));
					break;
				case 'd' :
					PrintDec(format, va_arg(args, int));
					break;
				case 'x' :
					PrintHex(format, va_arg(args, int));
					break;
				case 's' :
					PrintString(format, va_arg(args, char *));
 					break;
				case '%' :
					PrintChar("%c", '%');
					break;
            }
		}
		else 
		{
			PrintChar("%c", *s);
			s++;
		}
	}
	
	return;
}



void PrintChar(char *fmt, char c)
{
        print_byte_out(c);
        return;
}


void PrintDec(char *fmt, int l)
{
	int        i, j;
	char        c, *s=fmt, tol[10];
	bool        flag0=false, flagl=false;	// "%08lx"에서 '0', 'l'의 존재 여부.
	long        flagcnt=0;              	// "%08lx"에서 "8"을 찾아서 long형으로.
	bool        leading_zero=true;			// long형의 data를 출력하기 위한 변수.
	long        divisor, result, remainder;


	// fmt의 "%08lx"에서 '0', '8', 'l'을 해석.
	for (i=0; (c=s[i]) != 0; i++)
	{
		if (c=='d') break;
		else if (c>='1' && c<='9')
		{
			for (j=0; s[i]>='0' && s[i]<='9'; j++)
			{
				tol[j] = s[i++];
			}
			tol[j] = '\0';
			i--;
			DecToLong(tol, &flagcnt);
		}
		else if (c=='0') flag0=true;
		else if (c=='l') flagl=true;
		else continue;
	}

	// 위의 flag에 따라 출력.
	if (flagcnt)
	{
                if (flagcnt>9) flagcnt=9;
                remainder = l%(Power(10, flagcnt));        // flagcnt보다 윗자리의 수는 걸러냄. 199에 flagcnt==2이면, 99만.


                for (divisor=Power(10, flagcnt-1); divisor>0; divisor/=10){
                        result = remainder/divisor;
                        remainder %= divisor;


                        if (result!=0 || divisor==1) leading_zero = false;


                        if (leading_zero==true){
                                if (flag0)        print_byte_out('0');
                                else                print_byte_out(' ');
                        }
                        else print_byte_out((char)(result)+'0');
                }
        } else {
                remainder = l;


                for (divisor=1000000000; divisor>0; divisor/=10){
                        result = remainder/divisor;
                        remainder %= divisor;


                        if (result!=0 || divisor==1) leading_zero = false;
                        if (leading_zero==false) print_byte_out((char)(result)+'0');
                }
        }
        return;
}

void PrintHex(char *fmt, int l){
        int                i, j;
        char        c, *s=fmt, tol[10];
        bool        flag0=false, flagl=false;        // flags.
        long        flagcnt=0;
        bool        leading_zero=true;
        char        uHex, lHex;
        int                cnt;                                                // "%5x"의 경우 5개만 출력하도록 출력한 개수.


        // fmt의 "%08lx"에서 '0', '8', 'l'을 해석.
        for (i=0; (c=s[i]) != 0; i++){
                if (c=='x') break;
                else if (c>='1' && c<='9'){
                        for (j=0; s[i]>='0' && s[i]<='9'; j++){
                                tol[j] = s[i++];
                        }
                        tol[j] = '\0';
                        i--;
                        DecToLong(tol, &flagcnt);
                }
                else if (c=='0') flag0=true;
                else if (c=='l') flagl=true;
                else continue;
        }


        s = (char *)(&l);
        l = SWAP32(l);                // little, big endian에 따라서.(big이 출력하기 쉬워 순서를 바꿈)
        
        // 위의 flag에 따라 출력.
        if (flagcnt){
                if (flagcnt&0x01){        // flagcnt가 홀수 일때, upper를 무시, lower만 출력.
                        c = s[(8-(flagcnt+1))/2]; // 홀수 일때 그 위치를 포함하는 곳의 값을 가져 옵니다.
                        
                        // lower 4 bits를 가져와서 ascii code로.
                        lHex = ((c>>0)&0x0f);
                        if (lHex!=0) leading_zero=false;
                        if (lHex<10) lHex+='0';
                        else         lHex+='A'-10;


                        // lower 4 bits 출력.
                        if (leading_zero){
                                if (flag0) print_byte_out('0');
                                else       print_byte_out(' ');
                        }
                        else print_byte_out(lHex);
                        
                        flagcnt--;
                }


                // byte단위의 data를 Hex로 출력.
                for (cnt=0, i=(8-flagcnt)/2; i<4; i++){
                        c = s[i];
                                
                        // get upper 4 bits and lower 4 bits.
                        uHex = ((c>>4)&0x0f);
                        lHex = ((c>>0)&0x0f);


                        // upper 4 bits and lower 4 bits to '0'~'9', 'A'~'F'.
                        // upper 4 bits를 ascii code로.
                        if (uHex!=0) leading_zero = false;
                        if (uHex<10) uHex+='0';
                        else         uHex+='A'-10;


                        // upper 4 bits 출력.
                        if (leading_zero){
                                if (flag0) print_byte_out('0');
                                else       print_byte_out(' ');
                        }
                        else print_byte_out(uHex);
                        
                        // lower 4 bits를 ascii code로.
                        if (lHex!=0) leading_zero = false;
                        if (lHex<10) lHex+='0';
                        else         lHex+='A'-10;


                        // lower 4 bits 출력.
                        if (leading_zero){
                                if (flag0) print_byte_out('0');
                                else       print_byte_out(' ');
                        }
                        else print_byte_out(lHex);
                }
        }
        else {
                for (i=0; i<4; i++){
                        c = s[i];
        
                        // get upper 4 bits and lower 4 bits.
                        uHex = ((c>>4)&0x0f);
                        lHex = ((c>>0)&0x0f);


                        // upper 4 bits and lower 4 bits to '0'~'9', 'A'~'F'.
                        if (uHex!=0) leading_zero = false;
                        if (uHex<10) uHex+='0';
                        else         uHex+='A'-10;
                        if (!leading_zero) print_byte_out(uHex);
                        
                        if (lHex!=0 || i==3) leading_zero = false;
                        if (lHex<10) lHex+='0';
                        else         lHex+='A'-10;
                        if (!leading_zero) print_byte_out(lHex);
                }
        }
        return;
}

void PrintString(char *fmt, char *s)
{
        if (!fmt || !s) return;
        while (*s) print_byte_out(*s++);
        return;
}

int Power(int num, int cnt)
{
        long retval=num;
        cnt--;


        while (cnt--){
                retval *= num;
        }
        return retval;
} 
