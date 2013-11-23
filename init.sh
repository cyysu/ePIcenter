LAN_INT=eth0
LAN_IP=192.168.1.1

WLAN_INT=wlan0
WLAN_IP=192.168.2.1

WAN_INT=ppp0

sysctl -w net.ipv4.ip_forward=1

ifconfig $LAN_INT $LAN_IP up
ifconfig $WLAN_INT $WLAN_IP up


cp Conf/dnsmasq.conf /etc/
service dnsmasq restart

sh Firewall/firewall.sh

src/usbTask
