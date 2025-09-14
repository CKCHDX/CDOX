#!/bin/bash
# cdox_squashfs_setup.sh
# Run this INSIDE chroot of extracted squashfs
# It sets up everything for CDOX live system

set -euo pipefail

USERNAME="cdox"
CDOX_PKG_LIST="/cdox.txt"

log() { echo -e "\n[+] $*"; }

export DEBIAN_FRONTEND=noninteractive

# Update & install essentials
log "Updating system..."
apt-get update -y
apt-get upgrade -y

log "Installing essentials..."
apt-get install -y --no-install-recommends \
    sudo dbus dbus-x11 udev policykit-1 locales systemd-sysv sddm x11-xserver-utils \
    $(grep -vE '^\s*$|^\s*#' "$CDOX_PKG_LIST" | tr -d '\r')

# Create user
if ! id "$USERNAME" >/dev/null 2>&1; then
    log "Creating user $USERNAME"
    useradd -m -s /bin/bash "$USERNAME"
    echo "$USERNAME:$USERNAME" | chpasswd
    usermod -aG sudo,adm "$USERNAME"
else
    log "User $USERNAME already exists"
fi

# Configure SDDM autologin
log "Configuring SDDM autologin..."
mkdir -p /etc/sddm.conf.d
cat > /etc/sddm.conf.d/10-autologin.conf <<EOT
[Autologin]
User=$USERNAME
Session=cdox.desktop
EOT

# Create CDOX desktop entry
log "Creating CDOX session..."
mkdir -p /usr/share/xsessions
cat > /usr/share/xsessions/cdox.desktop <<'EOT'
[Desktop Entry]
Name=CDOX
Comment=Command Dashboard Overview X Session
Exec=/usr/bin/cdox-session.sh
Type=Application
EOT

# Session script
cat > /usr/bin/cdox-session.sh <<'EOT'
#!/bin/bash
if ! pgrep -x dbus-daemon > /dev/null; then
  eval $(dbus-launch --sh-syntax)
fi
exec /usr/bin/CDOX
EOT
chmod +x /usr/bin/cdox-session.sh

# Placeholder CDOX app if not present
if [ ! -f /usr/bin/CDOX ]; then
    log "Creating placeholder /usr/bin/CDOX"
    cat > /usr/bin/CDOX <<'EOT'
#!/bin/bash
echo "CDOX placeholder interface..."
exec /usr/bin/konsole || exec /bin/bash
EOT
    chmod +x /usr/bin/CDOX
fi

# Install live init script
log "Installing live init script..."
mkdir -p /usr/local/bin
cat > /usr/local/bin/cdox_live_init.sh <<'EOT'
#!/bin/bash
# cdox_live_init.sh
# Runs once at first live boot to finalize CDOX system

set -euo pipefail

log() { echo -e "\n[CDOX INIT] $*"; }

log "Generating locales..."
sed -i 's/^# *en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen
locale-gen
update-locale LANG=en_US.UTF-8

log "Cleaning apt cache..."
apt-get clean
rm -rf /var/lib/apt/lists/*

log "Disabling self after run..."
systemctl disable cdox-init.service || true
rm -f /usr/local/bin/cdox_live_init.sh

log "CDOX live system is initialized."
EOT
chmod +x /usr/local/bin/cdox_live_init.sh

# Install systemd unit for live init
cat > /etc/systemd/system/cdox-init.service <<'EOT'
[Unit]
Description=CDOX First Boot Init
After=multi-user.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/cdox_live_init.sh
RemainAfterExit=no

[Install]
WantedBy=multi-user.target
EOT

systemctl enable cdox-init.service

# Cleanup
log "Cleaning apt cache..."
apt-get clean
rm -rf /var/lib/apt/lists/*

log "CDOX squashfs configuration complete! You can safely repack squashfs."
