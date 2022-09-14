# Make sure you have Pillow library installed
# Windows: py -m pip install Pillow
# Linux: pip install Pillow

import os
from PIL import Image

# Change the below to your directory containing your images
image_dir = 'your_path_here'

# Janky converter code
os.chdir(image_dir)
for file in os.listdir(image_dir): 
    img = Image.open(file)
    file_name, file_ext = os.path.splitext(file)
    img.save(file_name+'.png')


