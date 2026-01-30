#!/bin/bash
# Push to GitHub with credentials

cd /mnt/c/Users/20mah/.gemini/antigravity/scratch/mushfiqur-os

echo "Pushing to GitHub..."
echo ""
echo "Repository: https://github.com/MD-Mushfiqur123/mushfiqur-os.git"
echo ""
echo "When prompted for credentials:"
echo "  Username: MD-Mushfiqur123"
echo "  Password: [Your Personal Access Token]"
echo ""
echo "Get token at: https://github.com/settings/tokens/new"
echo "  - Select 'repo' scope"
echo "  - Generate and copy the token"
echo ""

git push -u origin main

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ SUCCESS! Your OS is now on GitHub!"
    echo ""
    echo "View it at: https://github.com/MD-Mushfiqur123/mushfiqur-os"
    echo ""
fi
