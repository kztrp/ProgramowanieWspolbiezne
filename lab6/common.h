#define SIZE 512

typedef struct {
 int type; /* typ procesu: 1 PROD, 2 KONS */
 int pnr ; /* numer procesu */
 char text[SIZE]; /* tekst komunikatu */
 } ms_type;