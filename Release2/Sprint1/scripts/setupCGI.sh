#!/bin/sh

apt-get update
apt-get install apache2 -y
apt-get install php-fpm -y

VERSION_PHP=$(ls /etc/php)

sed -i '0,/listen = .*/ s/listen = .*/listen = 9000/' /etc/php/"$VERSION_PHP"/fpm/pool.d/www.conf

systemctl restart "php$VERSION_PHP-fpm"

a2enmod proxy_fcgi

cp scripts/000-default.conf /etc/apache2/sites-enabled/000-default.conf
systemctl restart apache2.service
