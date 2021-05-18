// Kalrav Srivastava
// ks874

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

struct node
{
	char *name;
	int size;
	struct node *next;
};

void put(struct node *new, struct node *old)
{
	if (old->next != NULL)
	{
		if ((new->size > old->size) && (new->size <= old->next->size))
		{
			new->next = old->next;
			old->next = new;
		}

		else
		{
			put(new, old->next);
		}
	}

	else
	{
		old->next = new;
	}
}

void rec(char *inPath, struct node *inNode)
{
	char nPath[10000];
	char lst[10000];

	struct dirent *path;
	struct stat *fd = malloc(sizeof(struct stat));

	DIR *dir = opendir(inPath);

	if (!dir)
	{
		return;
	}

	while ((path = readdir(dir)) != NULL)
	{
		if (path->d_type == DT_DIR)
		{
			// directly from the slides

			if (strcmp(path->d_name, ".") != 0 && strcmp(path->d_name, "..") != 0)
			{
				strcpy(nPath, inPath);
				strcat(nPath, "/");
				strcat(nPath, path->d_name);

				rec(nPath, inNode);
			}
		}

		else
		{
			//Sorting

			strcpy(lst, inPath);
			strcat(lst, "/");
			strcat(lst, path->d_name);

			lstat(lst, fd);

			if (inNode->size < (int)fd->st_size)
			{
				struct node *curr;
				char *buf = malloc(10000 * sizeof(char));

				snprintf(buf, 10000, "%d\t%s/%s\n", (int)fd->st_size, inPath, path->d_name);

				curr = malloc(sizeof(struct node));

				curr->name = buf;
				curr->size = (int)fd->st_size;
				curr->next = NULL;

				put(curr, inNode);
			}
		}
	}
	closedir(dir);
}

// Output
void printAll(struct node *head)
{
	printf("%s", head->name);

	if (head->next != NULL)
	{
		printAll(head->next);
	}
}

int main(int argc, char *argv[])
{
	struct node *init;
	init = malloc(sizeof(struct node));

	init->size = -1;
	init->name = NULL;	
	init->next = NULL;

	rec(argv[1], init);

	printAll(init->next);
}
