1. VirtualBox ���� network adaptor Bridge �� �ٲ�.
2. /etc/netplan/01-network-manager-all.yaml ����
# Let NetworkManager manage all devices on this system
network:
  ethernets:
    enp0s3:
      dhcp4: yes
#      addresses: [192.168.56.102/24]
    enp0s8:
      dhcp4: false
      addresses: [192.168.56.101/24]
      nameservers:
        addresses: [8.8.8.8, 4.2.2.2]
#      gateway4: 192.168.56.1
  version: 2
#  renderer: NetworkManager

3. $sudo netplan apply
--> Network / ������ �� �Ǵµ�, drag&drop �ȸ���.

