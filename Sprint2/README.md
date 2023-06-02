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

##

## Example

<pre>
 $ cat ./examples/get1  
GET / HTTP/1.1  
User-Agent: Wget/1.16 (linux-gnu)  
Accept: */*  
Host: www.google.com  
Connection: Keep-Alive
  
$ ./httpparser ./examples/get1 method   
searching for method  
FOUND [GET]  

$ ./httpparser ./examples/get1 header_field  
searching for header_field  
FOUND [User-Agent: Wget/1.16 (linux-gnu)]  
FOUND [Accept: */*]  
FOUND [Host: www.google.com]  
FOUND [Connection: Keep-Alive]  

$ ./httpparser ./get1
[0:HTTP_message] = "GET..__"
    [1:start_line] = "GET..1__"
        [2:request_line] = "GET..1__"
            [3:method] = "GET"
                [4:token] = "GET"
                    [5:tchar] = "G"
                        [6:__alpha] = "G"
                    [5:tchar] = "E"
                        [6:__alpha] = "E"
                    [5:tchar] = "T"
                        [6:__alpha] = "T"
            [3:__sp] = " "
            [3:request_target] = "/to..tml"
                [4:origin_form] = "/to..tml"
                    [5:absolute_path] = "/to..tml"
                        [6:__icar] = "/"
                        [6:segment] = "toto"
                            [7:pchar] = "t"
                                [8:unreserved] = "t"
                                    [9:__alpha] = "t"
                            [7:pchar] = "o"
                                [8:unreserved] = "o"
                                    [9:__alpha] = "o"
                            [7:pchar] = "t"
                                [8:unreserved] = "t"
                                    [9:__alpha] = "t"
                            [7:pchar] = "o"
                                [8:unreserved] = "o"
                                    [9:__alpha] = "o"
                        [6:__icar] = "/"
                        [6:segment] = "titi"
                            [7:pchar] = "t"
                                [8:unreserved] = "t"
                                    [9:__alpha] = "t"
                            [7:pchar] = "i"
                                [8:unreserved] = "i"
                                    [9:__alpha] = "i"
                            [7:pchar] = "t"
                                [8:unreserved] = "t"
                                    [9:__alpha] = "t"
                            [7:pchar] = "i"
                                [8:unreserved] = "i"
                                    [9:__alpha] = "i"
                        [6:__icar] = "/"
                        [6:segment] = "ind..tml"
                            [7:pchar] = "i"
                                [8:unreserved] = "i"
                                    [9:__alpha] = "i"
                            [7:pchar] = "n"
                                [8:unreserved] = "n"
                                    [9:__alpha] = "n"
                            [7:pchar] = "d"
                                [8:unreserved] = "d"
                                    [9:__alpha] = "d"
                            [7:pchar] = "e"
                                [8:unreserved] = "e"
                                    [9:__alpha] = "e"
                            [7:pchar] = "x"
                                [8:unreserved] = "x"
                                    [9:__alpha] = "x"
                            [7:pchar] = "."
                                [8:unreserved] = "."
                                    [9:__icar] = "."
                            [7:pchar] = "h"
                                [8:unreserved] = "h"
                                    [9:__alpha] = "h"
                            [7:pchar] = "t"
                                [8:unreserved] = "t"
                                    [9:__alpha] = "t"
                            [7:pchar] = "m"
                                [8:unreserved] = "m"
                                    [9:__alpha] = "m"
                            [7:pchar] = "l"
                                [8:unreserved] = "l"
                                    [9:__alpha] = "l"
            [3:__sp] = " "
            [3:HTTP_version] = "HTTP/1.1"
                [4:HTTP_name] = "HTTP"
                    [5:__num] = "HTTP"
                [4:__icar] = "/"
                [4:__digit] = "1"
                [4:__icar] = "."
                [4:__digit] = "1"
            [3:__crlf] = "__"
    [1:header_field] = "Use..nu)"
        [2:field_name] = "Use..ent"
            [3:token] = "Use..ent"
                [4:tchar] = "U"
                    [5:__alpha] = "U"
                [4:tchar] = "s"
                    [5:__alpha] = "s"
                [4:tchar] = "e"
                    [5:__alpha] = "e"
                [4:tchar] = "r"
                    [5:__alpha] = "r"
                [4:tchar] = "-"
                    [5:__icar] = "-"
                [4:tchar] = "A"
                    [5:__alpha] = "A"
                [4:tchar] = "g"
                    [5:__alpha] = "g"
                [4:tchar] = "e"
                    [5:__alpha] = "e"
                [4:tchar] = "n"
                    [5:__alpha] = "n"
                [4:tchar] = "t"
                    [5:__alpha] = "t"
        [2:__icar] = ":"
        [2:OWS] = " "
            [3:__sp] = " "
        [2:field_value] = "Wge..nu)"
            [3:field_content] = "W"
                [4:field_vchar] = "W"
                    [5:__vchar] = "W"
            [3:field_content] = "g"
                [4:field_vchar] = "g"
                    [5:__vchar] = "g"
            [3:field_content] = "e"
                [4:field_vchar] = "e"
                    [5:__vchar] = "e"
            [3:field_content] = "t"
                [4:field_vchar] = "t"
                    [5:__vchar] = "t"
            [3:field_content] = "/"
                [4:field_vchar] = "/"
                    [5:__vchar] = "/"
            [3:field_content] = "1"
                [4:field_vchar] = "1"
                    [5:__vchar] = "1"
            [3:field_content] = "."
                [4:field_vchar] = "."
                    [5:__vchar] = "."
            [3:field_content] = "1"
                [4:field_vchar] = "1"
                    [5:__vchar] = "1"
            [3:field_content] = "6 ("
                [4:field_vchar] = "6"
                    [5:__vchar] = "6"
                [4:__sp] = " "
                [4:field_vchar] = "("
                    [5:__vchar] = "("
            [3:field_content] = "l"
                [4:field_vchar] = "l"
                    [5:__vchar] = "l"
            [3:field_content] = "i"
                [4:field_vchar] = "i"
                    [5:__vchar] = "i"
            [3:field_content] = "n"
                [4:field_vchar] = "n"
                    [5:__vchar] = "n"
            [3:field_content] = "u"
                [4:field_vchar] = "u"
                    [5:__vchar] = "u"
            [3:field_content] = "x"
                [4:field_vchar] = "x"
                    [5:__vchar] = "x"
            [3:field_content] = "-"
                [4:field_vchar] = "-"
                    [5:__vchar] = "-"
            [3:field_content] = "g"
                [4:field_vchar] = "g"
                    [5:__vchar] = "g"
            [3:field_content] = "n"
                [4:field_vchar] = "n"
                    [5:__vchar] = "n"
            [3:field_content] = "u"
                [4:field_vchar] = "u"
                    [5:__vchar] = "u"
            [3:field_content] = ")"
                [4:field_vchar] = ")"
                    [5:__vchar] = ")"
    [1:__crlf] = "__"
    [1:header_field] = "Acc..*/*"
        [2:field_name] = "Accept"
            [3:token] = "Accept"
                [4:tchar] = "A"
                    [5:__alpha] = "A"
                [4:tchar] = "c"
                    [5:__alpha] = "c"
                [4:tchar] = "c"
                    [5:__alpha] = "c"
                [4:tchar] = "e"
                    [5:__alpha] = "e"
                [4:tchar] = "p"
                    [5:__alpha] = "p"
                [4:tchar] = "t"
                    [5:__alpha] = "t"
        [2:__icar] = ":"
        [2:OWS] = " "
            [3:__sp] = " "
        [2:field_value] = "*/*"
            [3:field_content] = "*"
                [4:field_vchar] = "*"
                    [5:__vchar] = "*"
            [3:field_content] = "/"
                [4:field_vchar] = "/"
                    [5:__vchar] = "/"
            [3:field_content] = "*"
                [4:field_vchar] = "*"
                    [5:__vchar] = "*"
    [1:__crlf] = "__"
    [1:header_field] = "Hos..com"
        [2:Host_header] = "Hos..com"
            [3:__istring] = "Host"
            [3:__icar] = ":"
            [3:OWS] = " "
                [4:__sp] = " "
            [3:Host] = "www..com"
                [4:uri_host] = "www..com"
                    [5:host] = "www..com"
                        [6:reg_name] = "www..com"
                            [7:unreserved] = "w"
                                [8:__alpha] = "w"
                            [7:unreserved] = "w"
                                [8:__alpha] = "w"
                            [7:unreserved] = "w"
                                [8:__alpha] = "w"
                            [7:unreserved] = "."
                                [8:__icar] = "."
                            [7:unreserved] = "g"
                                [8:__alpha] = "g"
                            [7:unreserved] = "o"
                                [8:__alpha] = "o"
                            [7:unreserved] = "o"
                                [8:__alpha] = "o"
                            [7:unreserved] = "g"
                                [8:__alpha] = "g"
                            [7:unreserved] = "l"
                                [8:__alpha] = "l"
                            [7:unreserved] = "e"
                                [8:__alpha] = "e"
                            [7:unreserved] = "."
                                [8:__icar] = "."
                            [7:unreserved] = "c"
                                [8:__alpha] = "c"
                            [7:unreserved] = "o"
                                [8:__alpha] = "o"
                            [7:unreserved] = "m"
                                [8:__alpha] = "m"
    [1:__crlf] = "__"
    [1:header_field] = "Con..ive"
        [2:Connection_header] = "Con..ive"
            [3:__istring] = "Con..ion"
            [3:__icar] = ":"
            [3:OWS] = " "
                [4:__sp] = " "
            [3:Connection] = "Kee..ive"
                [4:connection_option] = "Kee..ive"
                    [5:token] = "Kee..ive"
                        [6:tchar] = "K"
                            [7:__alpha] = "K"
                        [6:tchar] = "e"
                            [7:__alpha] = "e"
                        [6:tchar] = "e"
                            [7:__alpha] = "e"
                        [6:tchar] = "p"
                            [7:__alpha] = "p"
                        [6:tchar] = "-"
                            [7:__icar] = "-"
                        [6:tchar] = "A"
                            [7:__alpha] = "A"
                        [6:tchar] = "l"
                            [7:__alpha] = "l"
                        [6:tchar] = "i"
                            [7:__alpha] = "i"
                        [6:tchar] = "v"
                            [7:__alpha] = "v"
                        [6:tchar] = "e"
                            [7:__alpha] = "e"
    [1:__crlf] = "__"
    [1:__crlf] = "__"
</pre>