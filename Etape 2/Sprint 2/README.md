# HTTP Parseur

Parser HTTP requests coded with C  
`Sprint 2/Separation.txt` shows the separation of tasks within the group  
`Sprint 2/HTTP-message.xmind` is the mindmap of the HTTP ABNF  
`Sprint 2/httpParser` is an old version of the teacher's parser  
`Sprint 2/httpParser2` is the latest version of the teacher's parser  
We implemented more accepted fields than needed because we first used the oldest version of the parser. Tests are using the oldest version too.

## Authors

- [Mathis]
- [Hugo]
- [Nathan]
- [Josias]

## API Reference

### Get tree
```
  ./httpParseur <file>
```

| Parameter | Type   | Description                     |
|:----------|:-------|:--------------------------------|
| `file`    | `file` | **Required**. The file to parse |

##
### Find field in tree
```
  ./httpParseur <file> <field>
```

| Parameter | Type     | Description                                 |
|:----------|:---------|:--------------------------------------------|
| `file`    | `file`   | **Required**. The file to parse             |
| `field`   | `string` | **Required**. The field to find in the tree |

## Documentation

```c
  int parseur(char *message, int length);
```  

| Parameter  | Type     | Description         |
|:-----------|:---------|:--------------------|
| `message`  | `char *` | the string to parse |
| `length`   | `int *`  | the length to parse |
| _`output`_ | `int`    | error code          |

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

| Parameter  | Type       | Description                      |
|:-----------|:-----------|:---------------------------------|
| `start`    | `Node *`   | where to start the search        |
| `name`     | `char *`   | the field to find in the tree    |
| _`output`_ | `_Token *` | Linked list of field occurrences |

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

