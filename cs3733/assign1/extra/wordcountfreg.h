struct word {
    char value[100];
    int count;
    struct word *pnext;
};
struct word *newWord(char *word);
void printlist(struct word *list);
struct word *update(char word[],struct word *wordList);
int match(char *word,struct word *wordList);
