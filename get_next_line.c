#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif

char	*ft_strjoin(char *s1, char *s2)
{
	char	*s3;
	size_t	i = 0;
	size_t	j = 0;

    if (!s1)
        return strdup(s2);
    s3 = (char*)malloc(strlen(s1) + strlen(s2) + 1);
    while (s1[i])
    {
        s3[i] = s1[i];
        i++;
    }
    while (s2[j])
    {
        s3[i + j] = s2[j];
        j++;
    }
    s3[i + j] = '\0';
    free(s1);
    s1 = NULL;
    return (s3);
}
char *read_line (int fd, char *prev_line)
{
    char *new_line = NULL;
    ssize_t bytes_read = 0;
    char *buff = NULL;
    
    if (prev_line && (new_line = strchr(prev_line, '\n')))
        return (prev_line);
    buff = (char *)malloc(BUFFER_SIZE + 1);
    while ((bytes_read = read(fd, buff, BUFFER_SIZE)))
    {
        if (bytes_read == -1)
        {
            free(buff);
            free(prev_line);
            return (NULL);
        }
        buff[bytes_read] = '\0';
        prev_line = ft_strjoin(prev_line, buff);
        if ((new_line = strchr(buff, '\n')))
            break;
    }
    free(buff);
    return (prev_line);
}

char *set_next_line(char *prev_line)
{
    char *new_line = strchr(prev_line, '\n');
    if (!new_line)
        return strdup(prev_line);
    return (strndup(prev_line, new_line - prev_line + 1));
}

char *set_prev_line (char *prev_line)
{
    char *new_line = strchr(prev_line, '\n');
    if (!new_line)
    {
        free(prev_line);
        return (NULL);
    }
    char *new_prev_line = strdup(new_line + 1);
    free(prev_line);
    if (*new_prev_line)
        return new_prev_line;
    free(new_prev_line);
    return (NULL);
}

char *get_next_line (int fd)
{
    static char *prev_line;    
    char *next_line = NULL;

    if (fd < 0 || BUFFER_SIZE < 1)
        return (NULL);
    prev_line = read_line(fd, prev_line);
    if (!prev_line)
        return (NULL);
    next_line = set_next_line(prev_line);
    prev_line = set_prev_line(prev_line);
    return (next_line);
}

