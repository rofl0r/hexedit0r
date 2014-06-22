#ifndef HEXEDIT_H
#define HEXEDIT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <curses.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#if HAVE_SYS_MOUNT_H
#include <sys/mount.h> /* for BLKGETSIZE */
#endif

void LSEEK(int fd, off_t where);
int LSEEK_(int fd, off_t where);

/*******************************************************************************/
/* Macros */
/*******************************************************************************/
#define BIGGEST_COPYING (1 * 1024 * 1024)
#define BLOCK_SEARCH_SIZE (4 * 1024)
#define SECTOR_SIZE ((off_t) 512)
#ifndef CTRL
  #define CTRL(c) ((c) & 0x1F)
#endif
#define ALT(c) ((c) | 0xa0)
#define DIE(M) { exitCurses(); fprintf(stderr, M, progName); exit(1); }
#define FREE(p) if (p) free(p)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define NORMAL A_NORMAL
#define MARKED A_REVERSE
#define MODIFIED A_BOLD
#define ATTRPRINTW(attr, a) do { if (oldattr != (attr)) attrset(attr), oldattr = (attr); printw a; } while (0)
#define MAXATTRPRINTW(attr, a) do { if (oldattr & (~(attr))) attrset(attr & oldattr), oldattr &= (attr); printw a; } while (0)
#define PRINTW(a) ATTRPRINTW(NORMAL, a)
#ifndef getnstr
  #define getnstr(str, n) wgetnstr(stdscr, str, n)
#endif


/*******************************************************************************/
/* Configuration parameters */
/*******************************************************************************/
typedef enum { bySector, maximized, LAST } modeType;
typedef struct {
  int blocSize, lineLength, page;
} modeParams;

extern modeParams modes[LAST];
extern modeType mode;
extern int colored;
extern char *usage;

#define pressAnyKey "(press any key)"


/*******************************************************************************/
/* Pages handling */
/*******************************************************************************/
typedef struct _typePage {
  struct _typePage *next;

  off_t base;
  int size;
  unsigned char *vals;
} typePage;


/*******************************************************************************/
/* Global variables */
/*******************************************************************************/

extern off_t lastEditedLoc, biggestLoc, fileSize;
extern off_t mark_min, mark_max, mark_set;
extern off_t base, oldbase;
extern int normalSpaces, cursor, cursorOffset, hexOrAscii;
extern int cursor, blocSize, lineLength, colsUsed, page;
extern int isReadOnly, fd, nbBytes, oldcursor, oldattr, oldcursorOffset;
extern int sizeCopyBuffer, *bufferAttr;
extern char *progName, *fileName, *baseName;
extern unsigned char *buffer, *copyBuffer;
extern typePage *edited;

extern char *lastFindFile, *lastYankToAFile, *lastAskHexString, *lastAskAsciiString, *lastFillWithStringHexa, *lastFillWithStringAscii;


/*******************************************************************************/
/* Miscellaneous functions declaration */
/*******************************************************************************/
off_t getfilesize(void);
int key_to_function(int key);
void init(void);
void quit(void);
int tryloc(off_t loc);
void openFile(void);
void readFile(void);
int findFile(void);
int computeLineSize(void);
int computeCursorXCurrentPos(void);
int computeCursorXPos(int cursor, int hexOrAscii);
void updateMarked(void);
int ask_about_save(void);
int ask_about_save_and_redisplay(void);
void ask_about_save_and_quit(void);
int setTo(int c);
void setToChar(int i, unsigned char c);

/*******************************************************************************/
/* Pages handling functions declaration */
/*******************************************************************************/
void discardEdited(void);
void addToEdited(off_t base, int size, unsigned char *vals);
void removeFromEdited(off_t base, int size);
typePage *newPage(off_t base, int size);
void freePage(typePage *page);


/*******************************************************************************/
/* Cursor manipulation function declarations */
/*******************************************************************************/
int move_cursor(off_t delta);
int set_cursor(off_t loc);
int move_base(off_t delta);
int set_base(off_t loc);

/*******************************************************************************/
/* Curses functions declaration */
/*******************************************************************************/
void initCurses(void);
void exitCurses(void);
void display(void);
void displayLine(int offset, int max);
void clr_line(int line);
void displayCentered(char *msg, int line);
void displayOneLineMessage(char *msg);
void displayTwoLineMessage(char *msg1, char *msg2);
void displayMessageAndWaitForKey(char *msg);
int displayMessageAndGetString(char *msg, char **last, char *p, int p_size);
void ungetstr(char *s);
int get_number(off_t *i);


/*******************************************************************************/
/* Search functions declaration */
/*******************************************************************************/
void search_forward(void);
void search_backward(void);


/*******************************************************************************/
/* Mark functions declaration */
/*******************************************************************************/
void markRegion(off_t a, off_t b);
void unmarkRegion(off_t a, off_t b);
void markSelectedRegion(void);
void unmarkAll(void);
void markIt(int i);
void unmarkIt(int i);
void copy_region(void);
void yank(void);
void yank_to_a_file(void);
void fill_with_string(void);


/*******************************************************************************/
/* Small common functions declaration */
/*******************************************************************************/
int streq(const char *s1, const char *s2);
int strbeginswith(const char *a, const char *prefix);
int myceil(int a, int b);
off_t myfloor(off_t a, off_t b);
int setLowBits(int p, int val);
int setHighBits(int p, int val);
char *strconcat3(char *a, char *b, char *c);
int hexCharToInt(int c);
int not(int b);
#ifndef HAVE_MEMMEM
char *mymemmem(char *a, size_t sizea, char *b, size_t sizeb);
#else
#define mymemmem(a,b,c,d) memmem(a,b,c,d)
#endif
#ifndef HAVE_MEMRMEM
char *mymemrmem(char *a, size_t sizea, char *b, size_t sizeb);
#else
#define mymemrmem(a,b,c,d) memrmem(a,b,c,d)
#endif
int is_file(char *name);
int hexStringToBinString(char *p, size_t *l);

#endif  /* HEXEDIT_H */
