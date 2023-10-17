#ifndef DOG_H
#define DOG_H
char *getEnvironmentVariable(const char *name, char **environ);
char *getExecutiblePath(char *command, char **env);
int getmulticommands(char **splitedcommands, char *commands);
void removenewline(ssize_t byteRead, char *line);
#endif
