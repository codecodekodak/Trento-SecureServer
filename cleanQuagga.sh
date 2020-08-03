
sudo cp /var/lib/dpkg/statoverride /var/lib/dpkg/statoverride.bak
sudo grep -E "root" /var/lib/dpkg/statoverride > /tmp/statoverride
sudo cp /tmp/statoverride /var/lib/dpkg/statoverride

echo Done