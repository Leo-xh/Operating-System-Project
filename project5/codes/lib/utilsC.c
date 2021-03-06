#include "utilsC.h"
#include "utilsAsm.h"


extern struct info information[Len];
extern struct MemoryBlock memoryTable[Len];
extern struct info no;
extern int firstUnusedMemoryItem;
extern int line;
extern struct Control con;
extern struct record root;
extern short* FAT;
extern struct Process processTable[Len];
extern int curProcessId;
extern int ProcessSize;
extern int runSub;

__asm__(".code16gcc\n");

char * WelcomeSentence = "oh my Wsh\n\rCopyright (C) Xihuai Wang\n\rtype \"man\" to get help\n\r";
char * prompt = ">>";

// ×¢Òâ×Ö·û´®ÒªÓÐ'0'½áÎ²¡£
int strlen(char * sen)
{
	int i = 0;
	while(sen[i]) { ++i; }
	return i;
}

int countLines(char * sen)
{
	int lines = 0;
	for(int i = 0; sen[i]; ++i) {
		if(sen[i] == '\n') {
			++lines;
		}
	}
	lines += 1;
	return lines;
}

void initialScreen(int welcome)
{
	clear();
	if(welcome) {
		printSentence(WelcomeSentence, line, 0, strlen(WelcomeSentence), white);
		line += countLines(WelcomeSentence);
	}
	printSentence(prompt, line, 0, strlen(prompt), white);
	line += countLines(prompt);
}

void clear()
{
	line = 0;
	ClearScreen();
}

int strcmp(char * l, char * r)
{
	int i=0, j=0;
	for(; l[i] && r[i]; ++i, ++j)
	{
		if(l[i] < r[j]) {
			return -1;
		} else if(l[i] > r[j]) {
			return 1;
		} 
	}
	if(!l[i] && r[j]) {
		return -1;
	} else if(l[i] && !r[j]) {
		return 1;
	} else {
		return 0;
	}
}

void date()
{
	char * dateSen = getDate();
	printSentence(dateSen, line, 30, strlen(dateSen), purple);
	newline();
	line += countLines(dateSen);
}


void man()
{
	char * manual = getRecords(segOfOs, offsetOfManual);
	line = 0;
	ClearScreen();
	printSentence(manual, line, 0, strlen(manual), purple);
	line += countLines(manual);
}

void newline()
{
	if(line >= 25) {
		roll();
		line = 24;
	}
	printSentence(prompt, line, 0, strlen(prompt), white);
	line += countLines(prompt);
}



void strncpy(char * sour, char * des, int len)
{
	int i = 0;
	while(sour[i] && i < len){
		des[i] = sour[i];
		++i;
	}
	des[i] = '\0';
}

int hashfun(char * key){
	int weight = 29;
	int i = 0;
	int ret = 0;
	while(key[i]){
		ret = (ret + key[i]*weight) % Len;
		++i;
	}
	return ret;
}

int hash(char * key, struct info record)
{
	int inicode = hashfun(key);
	int code = inicode, i = 1;
	while(information[code].type!=null){
		code = (inicode + i * i) % Len;
		++i;
	}


	information[code].type = record.type;
	strncpy(record.name, information[code].name, strlen(record.name));
	information[code].size = record.size;
	information[code].lmaddress = record.lmaddress;
	information[code].deleted = record.deleted;
	return code;
}

int find(char * key)
{
	int inicode = hashfun(key);
	int code = inicode, i = 1;
	while(information[code].type == null || strcmp(key, information[code].name) != 0){
		code = (inicode + i * i) % Len;
		++i;
		if(i > Len)
			return -1;
	}
	return code;
}

void ls()
{
	char * head = "Name           Size           Lma            Type";
	printSentence(head, line, 10, strlen(head), green);
	char typeTable[10][1] = {"N", "D", "E", "F"};
	line += countLines(head);
	char tmp[15];
	for(int i = 0; i < Len; ++i) 
	{
		if(information[i].type != null && information[i].deleted != 1) {
			printSentence(information[i].name, line, 10, strlen(information[i].name), purple);
			int2str(information[i].size, tmp);
			printSentence(tmp, line, 25, strlen(tmp), purple);
			int2str(information[i].lmaddress, tmp);
			printSentence(tmp, line, 40, strlen(tmp), purple);
			printSentence(typeTable[information[i].type], line, 55, 1, purple);
			line += 1;
		}
	}
}

void int2str(int org, char * str)
{
	int num = 0;
	int cp = org;
	while(cp != 0)
	{
		cp /= 10;
		++num;
	}
	if(org == 0)
		num = 1;
	str[num--] = '\0';
	for(int i = num; i >= 0; --i)
	{
		str[i] = org % 10 + 48;
		org /= 10;
	}
}

void initialFile()
{
	FAT = (short*)getRecords(segOfOs, offsetOfFat);
	for(int i = 0; i < Len; ++i){
		information[i].type=null;
	}
	no.lmaddress = 0;
	no.size = 0;
	no.type = null;
	strncpy("", no.name, 0);
	loadFiles();
	

}


void loadFiles()
{
	char * rawRecords = getRecords(segOfOs, offsetOfRecord);
	int i = 0;
	int l = 0;
	int size = 0;
	int place = 0;
	char name[30];
	enum fileType t = null;
	struct info tmp;
	while(rawRecords[i] != '\n')
	{
		size = 0;
		place = 0;
		// name
		++i;
		l = 0;
		while(rawRecords[i]!='|'){
			++i;
			++l;
		}
		++i;
		strncpy(rawRecords+i-l-1, name, l);
		name[l] = '\0';
		// size
		while(rawRecords[i]!='|'){
			size = size * 10 + rawRecords[i]-'0';
			++i;
		}		
		++i;
		// place
		while(rawRecords[i]!='|'){
			place = place * 10 + rawRecords[i]-'0';
			++i;
		}		
		++i;
		//type
		t = rawRecords[i]-'0';
		i+=3;
		tmp.size = size;
		tmp.type = t;
		tmp.lmaddress = place;
		tmp.deleted = 0;
		strncpy(name, tmp.name, strlen(name));
		hash(name, tmp);
	}
}


void initialProcessTable()
{
	for (int i = 0; i < Len; ++i)
	{
		processTable[i].id = -1;
		processTable[i].status = Origin;
	}
	
	processTable[0].status = running;
	strncpy("kernel", processTable[0].name, 6); 
	processTable[0].id = 0;
	processTable[0].pcb.cs = 0x2000;
	processTable[0].pcb.ip = 0x0;
	processTable[0].pcb.ss_now = 0x2000;
	processTable[0].pcb.sp_now = 0xffff;
	// curProcessId = 1;
}

void ps()
{
	char * head = "Id  ProcessName       Status\n";
	char status[6][10] = {"Origin", "ready", "running", "blocked", "suspend", "exit"};
	int begin = 20;
	printSentence(head, line++, begin, strlen(head), purple);
	char str[30];
	for (int i = 0; i < Len; ++i)
	{
		if(processTable[i].id != -1)
		{
			int2str(processTable[i].id, str);
			printSentence(str, line, begin+1, strlen(str), purple);
			printSentence(processTable[i].name, line, begin+5, strlen(processTable[i].name), purple);
			printSentence(status[processTable[i].status], line++, begin+23, strlen(status[processTable[i].status]), purple);
		}
	}
}

int nextUnusedItem()
{
	for (int i = 0; i < MemLen; ++i)
	{
		if(memoryTable[i].used == 0)
			return i;
	}
	return -1;
}

void initialMemoryTable()
{
	int next;
	for (int i = 0; i < MemLen; ++i)
	{
		memoryTable[i].used = 0;
		memoryTable[i].next = -1;
		memoryTable[i].pre = -1;
		memoryTable[i].status = unused;
	}
	firstUnusedMemoryItem = 0;
	memoryTable[firstUnusedMemoryItem].pre = -1;
	memoryTable[firstUnusedMemoryItem].used = 1;
	memoryTable[firstUnusedMemoryItem].beginAddr = 0;
	memoryTable[firstUnusedMemoryItem].endAddr = 0x4ff;
	memoryTable[firstUnusedMemoryItem].status = used;
	next = nextUnusedItem();
	memoryTable[next].pre = firstUnusedMemoryItem;
	memoryTable[firstUnusedMemoryItem].next = next;
	firstUnusedMemoryItem = next;

	memoryTable[firstUnusedMemoryItem].used = 1;
	memoryTable[firstUnusedMemoryItem].beginAddr = 0x500;
	memoryTable[firstUnusedMemoryItem].endAddr = 0x7cff;
	memoryTable[firstUnusedMemoryItem].status = unused;
	next = nextUnusedItem();
	memoryTable[next].pre = firstUnusedMemoryItem;
	memoryTable[firstUnusedMemoryItem].next = next;
	firstUnusedMemoryItem = next;


	memoryTable[firstUnusedMemoryItem].used = 1;
	memoryTable[firstUnusedMemoryItem].beginAddr = 0x7e00;
	memoryTable[firstUnusedMemoryItem].endAddr = 0x7dff;
	memoryTable[firstUnusedMemoryItem].status = used;
	next = nextUnusedItem();
	memoryTable[next].pre = firstUnusedMemoryItem;
	memoryTable[firstUnusedMemoryItem].next = next;
	firstUnusedMemoryItem = next;

	memoryTable[firstUnusedMemoryItem].used = 1;
	memoryTable[firstUnusedMemoryItem].beginAddr = 0x7e00;
	memoryTable[firstUnusedMemoryItem].endAddr = 0xc1ff;
	memoryTable[firstUnusedMemoryItem].status = unused;
	next = nextUnusedItem();
	memoryTable[next].pre = firstUnusedMemoryItem;
	memoryTable[firstUnusedMemoryItem].next = next;
	firstUnusedMemoryItem = next;

	memoryTable[firstUnusedMemoryItem].used = 1;
	memoryTable[firstUnusedMemoryItem].beginAddr = 0xc200;
	memoryTable[firstUnusedMemoryItem].endAddr = 0xc9ff;
	memoryTable[firstUnusedMemoryItem].status = used;
	next = nextUnusedItem();
	memoryTable[next].pre = firstUnusedMemoryItem;
	memoryTable[firstUnusedMemoryItem].next = next;
	firstUnusedMemoryItem = next;

	memoryTable[firstUnusedMemoryItem].used = 1;
	memoryTable[firstUnusedMemoryItem].beginAddr = 0xca00;
	memoryTable[firstUnusedMemoryItem].endAddr = 0x1ffff;
	memoryTable[firstUnusedMemoryItem].status = unused;
	next = nextUnusedItem();
	memoryTable[next].pre = firstUnusedMemoryItem;
	memoryTable[firstUnusedMemoryItem].next = next;
	firstUnusedMemoryItem = next;

	memoryTable[firstUnusedMemoryItem].used = 1;
	memoryTable[firstUnusedMemoryItem].beginAddr = 0x20000;
	memoryTable[firstUnusedMemoryItem].endAddr = 0x2ffff;
	memoryTable[firstUnusedMemoryItem].status = used;
	next = nextUnusedItem();
	memoryTable[next].pre = firstUnusedMemoryItem;
	memoryTable[firstUnusedMemoryItem].next = next;
	firstUnusedMemoryItem = next;

	memoryTable[firstUnusedMemoryItem].used = 1;
	memoryTable[firstUnusedMemoryItem].beginAddr = 0x2ffff;
	memoryTable[firstUnusedMemoryItem].endAddr = 0x9ffff;
	memoryTable[firstUnusedMemoryItem].status = unused;
	next = nextUnusedItem();
	memoryTable[next].pre = firstUnusedMemoryItem;
	memoryTable[firstUnusedMemoryItem].next = next;
	firstUnusedMemoryItem = next;

}

int findItem(int addr)
{
	int itemId = 0;
	for(; itemId < MemLen; itemId = memoryTable[itemId].next)
	{
		if(memoryTable[itemId].beginAddr < addr)
			return itemId;
	}
	return -1;
}

void merge(int item)
{
	int first;
	for (first = item; memoryTable[first].pre != -1 &&
	 memoryTable[memoryTable[first].pre].status==unused; first=memoryTable[first].pre)
	{}
	while(memoryTable[first].next != -1 && memoryTable[memoryTable[item].next].status == unused)
	{
		int next = memoryTable[first].next;
		memoryTable[first].endAddr = memoryTable[next].endAddr;
		memoryTable[first].next = memoryTable[next].next;
		memoryTable[next].used = 0;
		next = memoryTable[next].next;
	}

}

void release(int begin, int end)
{
	int item = findItem(begin);
	memoryTable[item].status = unused;
	merge(item);
}

int findEnoughBlock(int size)
{
	int itemId = 0;
	// 不考虑退化情况
	while(memoryTable[itemId].status == used || memoryTable[itemId].endAddr - memoryTable[itemId].beginAddr + 1 < size)
	{
		itemId = memoryTable[itemId].next;
	}
	return itemId;
}


int require(int size, int curId)
{
	memoryTable[firstUnusedMemoryItem].beginAddr = memoryTable[curId].beginAddr;
	memoryTable[firstUnusedMemoryItem].endAddr = memoryTable[firstUnusedMemoryItem].beginAddr+size-1;
	memoryTable[firstUnusedMemoryItem].used = 1;
	memoryTable[firstUnusedMemoryItem].status = used;
	memoryTable[firstUnusedMemoryItem].pre = memoryTable[curId].pre;
	memoryTable[firstUnusedMemoryItem].next = curId;
	memoryTable[curId].pre = firstUnusedMemoryItem;
	memoryTable[curId].beginAddr = memoryTable[firstUnusedMemoryItem].endAddr+1;
	if(memoryTable[curId].beginAddr > memoryTable[curId].endAddr)
	{
		memoryTable[firstUnusedMemoryItem].next = memoryTable[curId].next;
		memoryTable[curId].used = 0;
	}
	int ret = firstUnusedMemoryItem;
	firstUnusedMemoryItem = nextUnusedItem();
	return ret;
}


//size要包括栈的大小
void createProcess(int id, char * name, int size, int fileId)
{
	processTable[id].id = id;
	processTable[id].status = ready;
	int blockNum = findEnoughBlock(size);
	blockNum = require(size, blockNum);
	processTable[id].blockNum = blockNum;
	strncpy(name, processTable[id].name, strlen(name));
	int offsetOfPrg, segOfPrg;
	int offsetMask = 0x0000ffff;
	int segMask = 0xf0000;
	offsetOfPrg = memoryTable[blockNum].beginAddr & offsetMask;
	segOfPrg = (memoryTable[blockNum].beginAddr & segMask) >> 4;

	//以下部分代码取巧，方法是直接改变cs和ss段（加上偏移量右移4位的值）以使得程序（org 0h）不需要org偏移量。 
	processTable[id].pcb.cs = segOfPrg + (offsetOfPrg >> 4);
	processTable[id].pcb.ip = 0;
	processTable[id].pcb.ss_now = segOfPrg + (offsetOfPrg >> 4);
	processTable[id].pcb.sp_now = size-1;
	loadReal(information[fileId].lmaddress, information[fileId].size, offsetOfPrg, segOfPrg);
}


void kill(int id)
{
	if(id != 0){
		processTable[id].status = exit;
		processTable[id].id = -1;
	}
	release(memoryTable[processTable[id].blockNum].beginAddr, memoryTable[processTable[id].blockNum].endAddr);

}

void int34h()
{
	char * s = "Int 34h Xihuai Wang";
	printSentence(s, 6, 5, strlen(s), purple);
}
void int35h()
{
	char * s = "Int 35h 16337236";
	printSentence(s, 6, 46, strlen(s), purple);
}
void int36h()
{
	char * s = "Int 34h Class two";
	printSentence(s, 18, 5, strlen(s), purple);
}
void int37h()
{
	char * s = "Int 34h Grade 2016";
	printSentence(s, 18, 46, strlen(s), purple);
}

void initial()
{
	initialFile();
	initialScreen(1);
	initialProcessTable();
	initialMemoryTable();
}

// void dispatch(struct Process *oldPro, struct Process *newPro)
// {
// 	oldPro->status = (newPro== &processTable[0]? blocked:ready);
// 	newPro->status = running;
// 	dispatchReal(&(oldPro->pcb), &(newPro->pcb));
// }

void schedule()
{
	if(runSub == 0)
	{
		curProcessId = 0;
		return ;
	} 
	else if(curProcessId != 0 ||(runSub == 1))
	{
		for(int i = (curProcessId+1)%ProcessSize; i !=curProcessId; i = (i+1)%ProcessSize)
		{
			if(i != 0 && processTable[i].status == ready)
			{
				processTable[i].status = running;
				processTable[curProcessId].status = ready;
				curProcessId = i;
				return ;
			}
		}
	}
}

void block()
{
	for (int i = 1; i < ProcessSize; ++i)
	{
		processTable[i].status = blocked;
	}
	runSub = 0;
	return ;
}

void wakeUp()
{
	for (int i = 1; i < ProcessSize; ++i)
	{
		if(processTable[i].status == blocked)
		{
			processTable[i].status = ready;
		}
		
	}
	return ;
}