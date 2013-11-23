########################################################################
################################FIREWALL################################
########################################################################
iptables -F
iptables -X
iptables -t nat -F
iptables -t nat -X
iptables -t mangle -F
iptables -t mangle -X

iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -P OUTPUT ACCEPT

#NAT
##############################
iptables -t nat -A POSTROUTING -o ppp0 -j MASQUERADE

