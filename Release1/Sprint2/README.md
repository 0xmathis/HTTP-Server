<pre>
 _    _   _______   _______   _____      _____               _____     _____   ______   _____  
| |  | | |__   __| |__   __| |  __ \    |  __ \      /\     |  __ \   / ____| |  ____| |  __ \  
| |__| |    | |       | |    | |__) |   | |__) |    /  \    | |__) | | (___   | |__    | |__) |  
|  __  |    | |       | |    |  ___/    |  ___/    / /\ \   |  _  /   \___ \  |  __|   |  _  /  
| |  | |    | |       | |    | |        | |       / ____ \  | | \ \   ____) | | |____  | | \ \  
|_|  |_|    |_|       |_|    |_|        |_|      /_/    \_\ |_|  \_\ |_____/  |______| |_|  \_\
</pre>


We coded a HTTP Parser requests coded with C  
[Separation.txt](doc/Separation.txt) shows the separation of tasks within the group  
[HTTP-message.xmind](doc/HTTP-message.xmind) is the mindmap of the HTTP ABNF  
[httpparser](bin/httpparser) is an old version of the teacher's parser  
[httpparser2](bin/httpparser2) is the latest version of the teacher's parser  
We implemented more accepted fields than needed because we first used the oldest version of the parser. Tests are using the oldest version too.

## Authors

- [Mathis COCAGNE](https://github.com/0xmathis/Projet-Reseau)
- Hugo HOARAU
- Nathan FRANCO
- Josias ADJETEY

## API Reference

### Get tree

```
  ./httpParseur <file>
```

| Parameter | Type   | Description                     |
|:----------|:-------|:--------------------------------|
| `file`    | `path` | **Required**. The file to parse |

##

### Find field in tree

```
  ./httpParseur <file> <field>
```

| Parameter | Type     | Description                                 |
|:----------|:---------|:--------------------------------------------|
| `file`    | `path`   | **Required**. The file to parse             |
| `field`   | `string` | **Required**. The field to find in the tree |

## Documentation

```c
  int parseur(char *message, int length);
```  

| Parameter | Type     | Description         |
|:----------|:---------|:--------------------|
| `message` | `char *` | the string to parse |
| `length`  | `int *`  | the length to parse |
| `output`  | `int`    | error code          |

##

```c
  void *purgeTree(void *root);
```

| Parameter | Type     | Description                  |
|:----------|:---------|:-----------------------------|
| `root`    | `Node *` | delete all the nodes created |

##

```c
  _Token *searchTree(void *start, char *name);
```

| Parameter | Type       | Description                      |
|:----------|:-----------|:---------------------------------|
| `start`   | `Node *`   | where to start the search        |
| `name`    | `char *`   | the field to find in the tree    |
| `output`  | `_Token *` | Linked list of field occurrences |

##

```c
  void *purgeElement(_Token **root);
```

| Parameter | Type        | Description                                           |
|:----------|:------------|:------------------------------------------------------|
| `root`    | `_Token **` | delete all the elements after root in the linked list |

## Running Tests

When running a test, the parser generates in the test folder:

- `.out` file : the output of the teacher's parser
- `.me` file : the output of our parser

Use `make clean` after testing to delete `.out` and `.me` files generated

##

### Check if the parser is able to parse valid files without error for simple files

```
  make testsSimple
```

Tested files are in `Tests/premier-jeu-test`

##

### Check if the parser's output is the same as the teacher's parser output for simple files

```
  make testsMedium
```

Tested files are in `Tests/premier-jeu-test`

##

### Check if the parser's output is the same as the teacher's parser output for hard files

```
  make testsHard
```

Tested files are in `Tests/Only4G33ks/testFile`

