#!/bin/bash
# Quick GitHub Publish Script

echo "========================================="
echo "  Publishing Mushfiqur OS to GitHub"
echo "========================================="
echo ""

# Check if git is configured
if ! git config user.name > /dev/null; then
    git config user.name "Mushfiqur Rahman"
    git config user.email "20mah@users.noreply.github.com"
fi

# Rename branch to main
git branch -M main

echo "Repository is ready to push!"
echo ""
echo "========================================="
echo "  STEP 1: Create GitHub Repository"
echo "========================================="
echo ""
echo "Go to: https://github.com/new"
echo ""
echo "Repository name: mushfiqur-os"
echo ""
echo "Description:"
echo "Custom Operating System with GUI - Built from Scratch in Assembly & C | x86 Bootloader | Protected Mode Kernel | VGA Graphics Driver | Window Manager"
echo ""
echo "Topics (add these):"
echo "operating-system, os-development, x86, assembly, bootloader, kernel, vga-graphics, window-manager, gui, systems-programming, osdev, bare-metal, c-programming, nasm, qemu"
echo ""
echo "✓ Public repository"
echo "✗ Do NOT initialize with README"
echo ""
echo "Click 'Create repository'"
echo ""
echo "========================================="
echo "  STEP 2: Copy Your GitHub Username"
echo "========================================="
echo ""
read -p "Enter your GitHub username: " username
echo ""

if [ -z "$username" ]; then
    echo "Error: Username required!"
    exit 1
fi

# Add remote
git remote remove origin 2>/dev/null
git remote add origin "https://github.com/$username/mushfiqur-os.git"

echo "========================================="
echo "  STEP 3: Push to GitHub"
echo "========================================="
echo ""
echo "Pushing to: https://github.com/$username/mushfiqur-os"
echo ""

# Push to GitHub
git push -u origin main

if [ $? -eq 0 ]; then
    echo ""
    echo "========================================="
    echo "  ✅ SUCCESS!"
    echo "========================================="
    echo ""
    echo "Your OS is now on GitHub!"
    echo ""
    echo "View it at: https://github.com/$username/mushfiqur-os"
    echo ""
    echo "Share it:"
    echo "  Twitter: https://twitter.com/intent/tweet?text=I%20built%20a%20complete%20OS%20from%20scratch!%20https://github.com/$username/mushfiqur-os"
    echo "  Reddit: https://reddit.com/r/osdev"
    echo ""
else
    echo ""
    echo "========================================="
    echo "  Authentication Required"
    echo "========================================="
    echo ""
    echo "GitHub requires authentication."
    echo ""
    echo "Option 1: Use GitHub CLI"
    echo "  gh auth login"
    echo "  Then run this script again"
    echo ""
    echo "Option 2: Use Personal Access Token"
    echo "  1. Go to: https://github.com/settings/tokens"
    echo "  2. Generate new token (classic)"
    echo "  3. Select 'repo' scope"
    echo "  4. Copy the token"
    echo "  5. Use it as password when pushing"
    echo ""
fi
