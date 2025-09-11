#!/bin/bash
# configure_cdox.sh
# Configure an installed Debian 13 minimal system into "CDOX".
# Run as root.
set -u

CDOX_PKG_LIST="./cdox.txt"
USERNAME="cdox"

log() { echo -e "\n[+] $*"; }
warn() { echo -e "\n[!] $*" >&2; }

if [ "$(id -u)" -ne 0 ]; then
  warn "This script must be run as root."
  exit 1
fi

if [ ! -f "$CDOX_PKG_LIST" ]; then
  warn "Cannot find $CDOX_PKG_LIST in current directory."
  exit 1
fi

# Read package list
mapfile -t PACKS < <(grep -vE '^\s*$|^\s*#' "$CDOX_PKG_LIST" | tr -d '\r')
log "Packages to process: ${#PACKS[@]}"

export DEBIAN_FRONTEND=noninteractive

# Update & upgrade system
log "Updating system..."
apt-get update -y && apt-get upgrade -y

# Install essentials (sudo, dbus, udev, policykit, locales, systemd)
log "Installing essentials..."
apt-get install -y sudo dbus udev policykit-1 locales systemd-sysv

# Create user if does not exist
if ! id "$USERNAME" >/dev/null 2>&1; then
  log "Creating user $USERNAME"
  useradd -m -s /bin/bash "$USERNAME"
  echo "$USERNAME:$USERNAME" | chpasswd
  usermod -aG sudo,adm "$USERNAME"
else
  log "User $USERNAME already exists"
fi

# Install all packages from cdox.txt
log "Installing packages from cdox.txt..."
for pkg in "${PACKS[@]}"; do
  log "Installing: $pkg"
  apt-get install -y --no-install-recommends "$pkg" || warn "Failed: $pkg"
done

# Ensure SDDM installed and enabled
log "Installing SDDM..."
apt-get install -y sddm
systemctl enable sddm.service
systemctl set-default graphical.target

# Configure SDDM autologin
log "Configuring SDDM autologin for $USERNAME..."
mkdir -p /etc/sddm.conf.d
cat > /etc/sddm.conf.d/10-autologin.conf <<EOT
[Autologin]
User=$USERNAME
Session=cdox.desktop
EOT

# CDOX session script
log "Creating CDOX session..."
cat > /usr/bin/cdox-session.sh <<'EOT'
#!/bin/bash
Xorg :0 &
sleep 3
kwin_x11 &
if ! pgrep -x dbus-daemon > /dev/null; then
  eval $(dbus-launch --sh-syntax)
fi
/usr/bin/CDOX
wait
EOT
chmod +x /usr/bin/cdox-session.sh

# CDOX desktop entry
mkdir -p /usr/share/xsessions
cat > /usr/share/xsessions/cdox.desktop <<'EOT'
[Desktop Entry]
Name=CDOX
Comment=Command Dashboard Overview X Session
Exec=/usr/bin/cdox-session.sh
Type=Application
EOT

# Provide placeholder /usr/bin/CDOX if not exists
if [ ! -f /usr/bin/CDOX ]; then
  log "Creating placeholder /usr/bin/CDOX"
  cat > /usr/bin/CDOX <<'EOT'
#!/bin/bash
echo "CDOX placeholder interface..."
exec /usr/bin/konsole || exec /bin/bash
EOT
  chmod +x /usr/bin/CDOX
fi

# Cleanup
log "Cleaning apt cache..."
apt-get clean
rm -rf /var/lib/apt/lists/*

log "Configuration complete! Reboot to start CDOX."
