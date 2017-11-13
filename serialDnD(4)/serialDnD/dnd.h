// dnd.h
// Some game constants
#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define MAX_NAME_LEN 20



typedef unsigned char byte;
typedef struct {
	byte map[MAP_WIDTH][MAP_HEIGHT];
	
} tDungeon;
typedef struct {
	char name[MAX_NAME_LEN+1];
	byte health;
	byte intelligence;	
	byte strength;
	byte magic;
	byte wealth;
	char x,y;
} tPlayer;

// Function prototypes
unsigned prbs();
int random(int range);
void ShowHelp();
void StartBanner();
void GameMessage(char *Msg);
char GetUserInput();
void RunGame(void);
void InitMap(tDungeon *Dungeon);
void ShowDungeon(tDungeon *Dungeon,tPlayer *thePlayer);
void InitPlayer(tPlayer *Player,tDungeon *Dungeon);
void ShowPlayer(tPlayer *thePlayer);
void Step(char Direction,tPlayer *Player,tDungeon *Dungeon);
