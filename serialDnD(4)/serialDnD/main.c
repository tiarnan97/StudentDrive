#include "uart.h"
#include "dnd.h"
// Find types: h(ealth),s(trength),m(agic),g(old)
char FindTypes[]={'h','s','m','g'};
// Baddie types : O(gre),T(roll),B(alrog)
char Baddies[]={'O','T','B'};
int GameStarted = 0;
tPlayer thePlayer;
tDungeon theDungeon;
void RunGame(void)
{
	char ch;
	StartBanner();
	ShowHelp();
	while(GameStarted == 0)
	{
		GameMessage("Press S to start a new game");
		ch = GetUserInput();			
		if ( (ch == 'S') || (ch == 's') )
			GameStarted = 1;
	}
	
	InitMap(&theDungeon);
	InitPlayer(&thePlayer,&theDungeon);	
	ShowPlayer(&thePlayer);
	GameMessage("You enter a darkened room......\r\n");
	
	while (1)
	{
		ch = GetUserInput();
		ch = ch | 32; // enforce lower case
		switch (ch) {
			case 'h' : {
				ShowHelp();
				break;
			}
			case 'n' : {
				GameMessage("Going north");
				break;
			}
			case 's' : {
				GameMessage("Going south");
				break;

			}
			case 'e' : {
				GameMessage("Going east");
				break;
			}
			case 'w' : {
				GameMessage("Going west");
				break;
			}
			case '#' : {				
				ShowDungeon(&theDungeon,&thePlayer);
				break;
			}
			case 'p' : {				
				ShowPlayer(&thePlayer);
				break;
			}
		} // end switch
	} // end while
}


void InitPlayer(tPlayer *Player,tDungeon *theDungeon)
{
	// get the player name
	int index=0;
	byte x,y;
	char ch=0;
	// Initialize the player's attributes
	printString("Enter the player's name: ");
	while ( (index < MAX_NAME_LEN) && (ch != '\r') )
	{
		ch = GetUserInput();
		if (ch != '\r')
		{
			eputc(ch);
			Player->name[index++]=ch;
		}
	}
	Player->name[index]=0; // terminate the name
	Player->health=100;
	Player->strength=50+random(50);
	Player->magic=50+random(50);
	Player->intelligence=50+random(50);
	Player->wealth=10+random(10);
	
	// Initialize the player's location
	// Make sure the player does not land
	// on an occupied space to begin with
	do {
		x=random(MAP_WIDTH);
		y=random(MAP_HEIGHT);
		
	} while(theDungeon->map[x][y] != '.');
	Player->x=x;
	Player->y=y;
}
void ShowPlayer(tPlayer *thePlayer)
{
	printString("\r\nPlayer details\r\n");
	printString(thePlayer->name);
	printString("\r\n");
	printString("health: ");
	printHex(thePlayer->health);
	printString("\r\nstrength: ");
	printHex(thePlayer->strength);
	printString("\r\nmagic: ");
	printHex(thePlayer->magic);
	printString("\r\nintelligence: ");
	printHex(thePlayer->intelligence);
	printString("\r\nwealth: ");
	printHex(thePlayer->wealth);	
	printString("\r\nLocation : ");
	printHex(thePlayer->x);
	printString(" , ");
	printHex(thePlayer->y);
	printString("\r\n");
	
}
void InitMap(tDungeon *Dungeon)
{
	int x,y;
	// clear the map to begin with
	for (x=0;x < MAP_WIDTH; x++)
	{
		for (y=0; y < MAP_HEIGHT; y++)
		{
			Dungeon->map[x][y] = '.';
		}
	}
	// insert some obstacles
	for (x=0;x < MAP_WIDTH; x++)
	{
		for (y=0; y < MAP_HEIGHT; y++)
		{
			if (random(100) > 90)
				Dungeon->map[x][y]='*';  
		}
	}
	// insert some goodies
	for (x=0;x < MAP_WIDTH; x++)
	{
		for (y=0; y < MAP_HEIGHT; y++)
		{
			if (random(100) > 95)
				Dungeon->map[x][y]=	FindTypes[random(sizeof(FindTypes)-1)];
		}
	}		
	// insert some baddies
	for (x=0;x < MAP_WIDTH; x++)
	{
		for (y=0; y < MAP_HEIGHT; y++)
		{
			if (random(100) > 98)
				Dungeon->map[x][y]=	Baddies[random(sizeof(Baddies)-1)];
		}
	}		

	// finally put the exit to the next level in
	x = random(MAP_WIDTH);
	y = random(MAP_HEIGHT);
	Dungeon->map[x][y]='X';
}
void ShowDungeon(tDungeon *Dungeon,tPlayer *thePlayer)
{
	int x,y;
	printString("\r\nThe dungeon: \r\n");
	for (y=0;y<MAP_HEIGHT;y++)
	{
		for (x=0;x<MAP_WIDTH;x++)
		{
			
			if ( (x==thePlayer->x) && (y==thePlayer->y))
				eputc('@');
			else
				eputc(Dungeon->map[x][y]);

		}
		eputc('\r');
		eputc('\n');
	}
}
void ShowHelp()
{
	printString("********************************************************\r\n");
	printString("****           Available commands:                  ****\r\n");
	printString("**** N,S,E,W : go north, south, east, west          ****\r\n");
	printString("**** P : show player details                        ****\r\n");
	printString("**** I : print inventory                            ****\r\n");
	printString("**** D : drop item                                  ****\r\n");	
	printString("**** W : wield item                                 ****\r\n");
	printString("**** C : Cast spell                                 ****\r\n");
	printString("********************************************************\r\n");
	printString("\r\nReady\r\n");
}

void StartBanner()
{
	printString("********************************************************\r\n");
	printString("****      Welcome to DnD on the LPC1114             ****\r\n");
	printString("********************************************************\r\n");
	printString("\r\n");
	
}
void GameMessage(char *Msg)
{
	printString(Msg);
	printString("\r\nReady\r\n");	
}
char GetUserInput()
{
	char ch = 0;
	while (ch == 0)
		ch = egetc();
	return ch;
}
int random(int range)
{
	return (prbs() % range);
}

void Step(char Direction,tPlayer *Player,tDungeon *Dungeon)	
	{
		int NewX,NewY;
		NewX = Player-> x;
		NewY = Player-> y;
		switch (Direction)
		{
			case 'n': NewY--;break;
			case 's': NewY++;break;
			case 'e': NewX++;break;
			case 'w': NewX--;break;
		}
		//check to see if its within the bounds of the dungeon
		if (NewX < 0) return;
		if (NewY < 0) return;
		if (NewX > 19) return;
		if (NewY > 19) return;
		//check tosee if somethings in the spot we're moving to
	if (Dungeon->map[NewX][NewY]=='*')
		//theres a rock in the way
		{
			GameMessage("ouch!");
			return;
		}
		
		//move not rejected s
		
		
	}