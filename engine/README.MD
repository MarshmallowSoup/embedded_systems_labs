# Install AVR Toolchain
```sh  
 sudo apt update
 sudo apt install gcc-avr avr-libc binutils-avr avrdude
```
#### add user to the dialout group
```sh
 sudo usermod -a -G dialout $USER
```
(Restart of the computer will be neaded to have changes applied)

## Install VSCode
```sh  
 sudo apt update
 sudo apt install snapd
 sudo snap install --classic code
```
#### or
```sh  
wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
sudo install -o root -g root -m 644 packages.microsoft.gpg /etc/apt/trusted.gpg.d/
sudo sh -c 'echo "deb [arch=amd64,arm64,armhf signed-by=/etc/apt/trusted.gpg.d/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" > /etc/apt/sources.list.d/vscode.list'
```
#### Then update the package cache and install the package using:
```sh
 sudo apt install apt-transport-https
 sudo apt update
 sudo apt install code  #or code-insiders
```

Install VS Code extensions
```sh
 code --install-extension jkearins.action-buttons-ext
 code --install-extension ms-vscode.cpptools
```

#### Install python and miniterm
```sh
sudo apt update
sudo apt install python-pip
sudo pip install pyserial
```