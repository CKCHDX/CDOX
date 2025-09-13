#!/bin/bash

PKG_FILE="packages.txt"

# Check if file exists
if [[ ! -f "$PKG_FILE" ]]; then
    echo "❌ Package list file '$PKG_FILE' not found!"
    exit 1
fi

# Update package list first
echo "🔄 Updating package list..."
sudo apt-get update -y

# Loop through each package
while IFS= read -r pkg || [[ -n "$pkg" ]]; do
    # Skip empty lines or comments
    [[ -z "$pkg" || "$pkg" =~ ^# ]] && continue

    echo "📦 Installing: $pkg"
    if sudo apt-get install -y "$pkg"; then
        echo "✅ Successfully installed: $pkg"
    else
        echo "⚠️ Failed to install: $pkg — skipping..."
    fi
done < "$PKG_FILE"

echo "🎯 All done!"
