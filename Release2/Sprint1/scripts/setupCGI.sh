#!/bin/sh

apt-get update
apt-get install apache2 -y
apt-get install php-fpm -y

MY_USER=userir
MY_GROUP=userir
VERSION_PHP=$(ls /etc/php)

sed -i '0,/listen = .*/ s/listen = .*/listen = 9000/' /etc/php/"$VERSION_PHP"/fpm/pool.d/www.conf
sed -i "0,/user = .*/ s/user = .*/user = $MY_USER/" /etc/php/"$VERSION_PHP"/fpm/pool.d/www.conf
sed -i "0,/group = .*/ s/group = .*/group = $MY_GROUP/" /etc/php/"$VERSION_PHP"/fpm/pool.d/www.conf

systemctl restart "php$VERSION_PHP-fpm"

a2enmod proxy_fcgi

cp scripts/000-default.conf /etc/apache2/sites-enabled/000-default.conf
systemctl restart apache2.service
