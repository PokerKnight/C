#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

struct tnode {	/* the tree node: */
	char *word;	/* points to the text */
	int count;	/* number of occurrences */
	struct tnode *left; 	/* left child */
	struct tnode *right;	/* right child */
};

#define MAXWORD 100

struct tnode *addtree(struct tnode *p, char *w);
void treeprint(struct tnode *p);
void treefree(struct tnode *p);
struct tnode *talloc(void);
char *my_strdup(char *s);
int getword(char *word, int lim);
int getch(void);
void ungetch(int c);

/* word frequency count */
int main()
{
	struct tnode *root;
	char word[MAXWORD];

	root = NULL;
	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			root = addtree(root, word);
	treeprint(root);
	treefree(root);

	return 0;
}

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond = 0;

	if (p == NULL)		/* a new word has arrived */
	{
		p = talloc();	/* make a new node */
		p->word = my_strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	}
	else if ((cond = strcmp(w, p->word)) == 0)
		p->count++;		/* repeated word */
	else if (cond < 0)	/* less than into left subtree */
		p->left = addtree(p->left, w);
	else				/* greater than into right subtree */
		p->right = addtree(p->right, w);

	return p;
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p)
{
	if (p != NULL)
	{
		treeprint(p->left);
		printf("%4d %s\n", p->count, p->word);
		treeprint(p->right);
	}
}

void treefree(struct tnode *p)
{
	if (p != NULL)
	{
		if (p->left);
			treefree(p->left);
		if (p->right);
			treefree(p->right);
		free(p->word);
		free(p);
	}
}

/* talloc: make a tnode */
struct tnode *talloc(void)
{
	return (struct tnode *)malloc(sizeof(struct tnode));
}

char *my_strdup(char *s)  /* make a duplicate of s */
{
	char *p;

	p = (char *)malloc(strlen(s)+1);  /* +1 for '\0' */
	if (p != NULL)
		strcpy(p, s);
	return p;
}

/* getword: get next word or character from input */
int getword(char *word, int lim)
{
	int c = 0;
	char *w = word;

	while (isspace(c = getch()))
		;
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c))
	{
		*w = '\0';
		return c;
	}
	for (; --lim > 0; w++)
		if (!isalnum(*w = getch()))
		{
			ungetch(*w);
			break;
		}
	*w = '\0';

	return word[0];
}

#define BUFSIZE	100

char buf[BUFSIZE] = {0};
int bufp = 0;

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

