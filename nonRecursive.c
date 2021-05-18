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

struct queue
{
	char *drct;
	struct queue *next;
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

void nrec(struct node *inNode, struct queue *qNode)
{
	struct dirent *path;
	struct stat *fd = malloc(sizeof(struct stat));
	DIR *dir;

	struct queue *current;
	current = malloc(sizeof(struct queue));

	current->drct = qNode->drct;
	current->next = NULL;

	// when queue not Null
	while (current != NULL)
	{
		char *current_dir = current->drct;
		current = current->next;
		dir = opendir(current_dir);
		if (!dir)
		{
			return;
		}

		while ((path = readdir(dir)) != NULL)
		{
			char *nPath = malloc(1001);
			nPath[1000] = 0;

			strcpy(nPath, current_dir);
			strcat(nPath, "/");
			strcat(nPath, path->d_name);

			if (path->d_type == DT_DIR)
			{
				// directly from the slides
				if (strcmp(path->d_name, ".") != 0 && strcmp(path->d_name, "..") != 0)
				{
					struct queue *q;
					q = malloc(sizeof(struct queue));

					q->drct = nPath;
					q->next = current;
					current = q;
				}
			}
			else
			{
				lstat(nPath, fd);

				if ((int)fd->st_size > inNode->size)
				{
					struct node *curr;
					char *buf = malloc(1000 * sizeof(char));

					snprintf(buf, 1000, "%d\t%s/%s\n", (int)fd->st_size, current_dir, path->d_name);

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
}

void prnt(struct node *head)
{
	printf("%s", head->name);

	if (head->next != NULL)
	{
		prnt(head->next);
	}
}

int main(int argc, char *argv[])
{
	struct node *init;
	init = malloc(sizeof(struct node));
	init->size = -1;


	struct queue *qinit;
	qinit = malloc(sizeof(struct queue));
	qinit->drct = argv[1];
	qinit->next = NULL;

	nrec(init, qinit);
	prnt(init->next);
}
