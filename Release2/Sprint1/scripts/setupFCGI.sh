#!/bin/bash

echo Installing packages
apt-get update 1> /dev/null
apt-get install apache2 -y 1> /dev/null
apt-get install php-fpm -y 1> /dev/null

OWNERSHIP=($(stat -c "%U %G" sites))
MY_USER=${OWNERSHIP[0]}
MY_GROUP=${OWNERSHIP[1]}
VERSION_PHP=$(ls /etc/php)

sed -i '0,/listen = .*/ s/listen = .*/listen = 9000/' /etc/php/"$VERSION_PHP"/fpm/pool.d/www.conf
sed -i "0,/user = .*/ s/user = .*/user = $MY_USER/" /etc/php/"$VERSION_PHP"/fpm/pool.d/www.conf
sed -i "0,/group = .*/ s/group = .*/group = $MY_GROUP/" /etc/php/"$VERSION_PHP"/fpm/pool.d/www.conf

echo Restarting PHP
systemctl restart "php$VERSION_PHP-fpm" 1> /dev/null

echo Restarting Apache2
a2enmod proxy_fcgi 1> /dev/null
cp scripts/apache2.conf /etc/apache2/sites-enabled/000-default.conf 1> /dev/null
systemctl restart apache2.service 1> /dev/null

echo FCGI enabled