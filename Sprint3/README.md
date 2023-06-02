<pre>
 _    _   _______   _______   _____       _____   ______   _____  __      __  ______   _____  
| |  | | |__   __| |__   __| |  __ \     / ____| |  ____| |  __ \ \ \    / / |  ____| |  __ \  
| |__| |    | |       | |    | |__) |   | (___   | |__    | |__) | \ \  / /  | |__    | |__) |  
|  __  |    | |       | |    |  ___/     \___ \  |  __|   |  _  /   \ \/ /   |  __|   |  _  /  
| |  | |    | |       | |    | |         ____) | | |____  | | \ \    \  /    | |____  | | \ \  
|_|  |_|    |_|       |_|    |_|        |_____/  |______| |_|  \_\    \/     |______| |_|  \_\
</pre>

We coded a WEB server in C  
[server](bin/server) is the server's executable
While running, the server listen for HTTPrequests on the port 8080
The highest HTTP version implemented is HTTP/1.1

## Authors

- [Mathis COCAGNE](https://github.com/0xmathis/Projet-Reseau)
- Hugo HOARAU
- Nathan FRANCO
- Josias ADJETEY
## Run Locally
All the following commands need root

### Domain Names

To be able to “own domain names” we need to edit the `/etc/hosts` file
```
  make prepa
```
This command will create local domains names  
These custom domain names can be set in [scripts/hosts](scripts/hosts)

To unset the modifications
```
  make unprepa
```

##

### Set up the fastCGI interface
To be able to run PHP files we need to create an fastCGI interface
```
  make setupFCGI
```

##

### Execute
Execute the serveur on Linux
```
  make run
```

## Documentation

We created 2 basics test Websites  
`site1.fr:8080` contains images, CSS, JS scripts, PHP scripts  
`site2.fr:8080` contains GIFS, video file, audio file, JS scripts

For each video or audio file request, the server send a partial answer (206) so that the client doesn't need to download the full video (i.e. music) file to watch it
To do this, we implemented the Range headers  
You can now watch a long video without loading all of it