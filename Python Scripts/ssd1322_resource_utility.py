# -*- coding: utf-8 -*-
"""
========================
SSD1322 RESOURCE UTILITY
========================

This program provides functions for generating code from 
bitmap (.bmp) and font (.tty) files for applications using the 
SSD1322 OLED driver. The generated files are source (.c) and header (.h) files 
for use in embedded applications.

See adomkwabena.com/2019/06/01/ssd1322-oled-fun:-part-one for more information
"""

from fontdemo import Font
import matplotlib.image as mpimg
import string

def flatten(data):
    """
    Converts a nested list of unknown depth into a single list.
    
    data (list): a nested list of unknown depth.
    
    Returns (list): a flattened copy of data.
    """
    flattened = []
    for i in data:
        flattened += flatten(i) if type(i) == list else [i]
        
    return flattened

def read_bitmap(filename):
    """
    Reads a bitmap file and generates an array of pixel data.
    
    filename (str): The filename of the bitmap.
                    Ensure the bitmap is in the working directory.
                    
    Returns (tuple): A tuple which contains image data.
                     The tuple items are:
                         rows (int)
                         columns (int)
                         bitmap_array (list)
    """
    # Read image and scale grayscale values from 8bit to 4bit
    img = mpimg.imread(filename) // 16
    # Unpack image properties
    rows, columns = img.shape

    bitmap_array = img.tolist()
    bitmap_array = flatten(bitmap_array)
  
    return (rows, columns, bitmap_array)

def add_dummy_data(bitmap, width, height):
    """
    Adds dummy data to a bitmap to ensure the width is divisible by 4. 
    This is required because the SSD1322 OLED driver maintains a single column 
    address for four pixels.
    
    bitmap (list): A list of integers (pixel values)
    width (int): The width of the bitmap
    height (int): The height of the bitmap
    
    Returns (tuple): A tuple which contains modified image data.
                     |
                     |--> bitmap_data (tuple)
                     |    |--> bitmap pixel array (list)
                     |    |--> bitmap width  (int)
                     |    \--> bitmap height (int)
                     |
                     \--> dummy_to_add (int): Columns of dummy data added
    """
    output = []
    bitmap_data = tuple()
    # Maximum dummy data should be 3 
    dummy_to_add = (4 - (width % 4)) % 4
    
    new_width = width + dummy_to_add
    
    if dummy_to_add > 0:
        index = 0
        for i in range(height):
            for j in range(new_width):
                # Add dummy data beyond original width
                if j > (width - 1):
                    output.append(0)
                else:
                    output.append(bitmap[index])
                    index += 1
        bitmap_data = (output, new_width, height)
    else:
        bitmap_data = (bitmap, width, height)
    
    return (bitmap_data, dummy_to_add)
    
def format_bitmap(bitmap):
    """
    Combines two adjacent bytes into one byte.
    This is required because the SSD1322 OLED driver represents two pixels
    in one byte with each pixel being 4 bits wide. Call add_dummy_data() before 
    calling this function to ensure the incoming data is even in width.
    
    bitmap (list): A list containing pixel data - each byte contains one pixel
                   but each pixel occupies only half of a byte.
    
    returns (list): A list of hex strings - each byte is two pixels.
    """
    output = [] 
    index = 0
    
    size = len(bitmap) - 1
    while index < size:
        # Merge two bytes(pixels) into one
        output.append((bitmap[index] << 4) + bitmap[index + 1])
        # move on to the next two bytes
        index += 2
    # Representing data in string format makes for easier code generation
    return ["0x%02X" % i for i in output]
  
def bitmap_to_array(bitmaps):
    """
    Generates an 'array' representation of the bitmap data.
    This data is then used to generate a header (.h) file and a source (.c) file.
    
    bitmaps (list): An array of the filenames of the (.bmp) files to be
                    converted.
                    
    returns (dict): 
        A nested dictionary containing bitmap data.
            bitmap_table (dict)
            |
            \--> bitmap name (str) : bitmap parameters (dict)
                                     |--> 'Width'  : bitmap width (int)
                                     |--> 'Height' : bitmap height (int)
                                     \--> 'Bitmap' : bitmap pixel array (list)
    """
    bitmap_table = {}
    
    # Create a translation table for the filenames
    t = {ord(i) : '_' for i in string.punctuation}
    
    for filename in bitmaps:
        # Ensure filename is a valid C variable name.
        file = filename.split('.')[0].lstrip(string.digits)
        file = file.translate(t)
        
        bitmap_data = tuple()
        rows, columns, data = read_bitmap(filename)
        
        # Ensure bitmap width is divisible by 4.
        bitmap_data, dummy_added = add_dummy_data(data, columns, rows)
        # Merge adjacent pixels into single bytes
        bitmap = format_bitmap(bitmap_data[0])
        # Represent the width of the bitmap as the SSD1322 does, where each
        # column address represents 4 pixels.This requires a division by 2 (each byte represents 2 pixels)
        # but we divide by 4 because format_bitmap() halves the width of the bitmap.
        width = bitmap_data[1] // 4
        height = bitmap_data[2]
        
        bitmap_table[file] = {
                              'Width'  : width, 
                              'Height' : height, 
                              'Bitmap' : bitmap,
                             }
    
    return bitmap_table

def bitmap_to_c(bitmaps, filename="resources"):
    """
    Generates a header (.h) file and source (.c) file in the working directory. 
    
    filename (str): The filename of the output files (.c and .h files).
    
    bitmaps (dict): A dict of ".bmp" filenames to be converted. The bitmap
                    files have to be in the active directory.
                    
    Returns: None
    """
    bitmap_table = bitmap_to_array(bitmaps)
    # Generate header file
    with open(filename + ".h", mode="w") as f:
        f.write("/**\n"
            " * @File Name\n"
            " *   " + filename + ".h\n *\n"
            " * @Description\n"
            " *   This header file provides access to the installed bitmap\n"
            " *   This code was auto generated with ssd13322_resource_utility.py\n"
            " */\n\n"
            "/**\n"
            " * Section: Included Files\n"
            " */\n\n"
            "#include <stdint.h>\n"
            "#include \"ssd1322.h\"\n\n")
    
        for bmp in bitmap_table:
            f.write("// Bitmap Structure\n"
                    "extern const bitmap_t " + bmp + ";\n\n")
            
    # Generate source file
    with open(filename + ".c", mode="w") as f:
        f.write("/**\n"
                " * @File Name\n"
                " *   " + filename + ".c\n *\n"
                " * @Description\n"
                " *   This source file contains pixel data of the installed bitmap\n"
                " *   This code was auto generated with ssd1322_resource_utility.py\n"
                " */\n\n"
                "/**\n"
                " * Section: Included Files\n"
                " */\n\n"
                "#include \"ssd1322.h\"\n\n"
                "/**\n"
                " * Section: Module Definitions\n"
                " */\n\n")
        
        # Create bitmap size definitions for each bitmap file
        for i, bmp in enumerate(bitmap_table):
            w = bitmap_table[bmp]['Width']
            h = bitmap_table[bmp]['Height']
            
            f.write("#define BITMAP_" + str(i) + "_WIDTH    " + str(w) + "u\n"
                    "#define BITMAP_" + str(i) + "_HEIGHT   " + str(h) + "u\n\n")       
        
        # Create bitmap body for each bitmap file
        for bmp in bitmap_table:
            rows = bitmap_table[bmp]['Height']
            bitmap = bitmap_table[bmp]['Bitmap']
            
            # Write bitmap data to file
            f.write("/**\n"
                " * Section: Bitmap Body\n"
                " */\n\n"
                "// Each byte represents two pixels\n"
                "const uint8_t " + bmp + "_bitmap[] =\n{\n")
        
            # Write bitmap data in widths of 12 bytes each
            size = len(bitmap)
            rows = size // 12
            remainder = size % 12
            index = 0
        
            for i in range(rows):
                f.write('    ')
                for j in range(12):
                    f.write(bitmap[index] + ', ' )
                    index += 1
                f.write("\n")                
            
            if remainder > 0:
                f.write('    ')
                for i in range(remainder):
                    f.write(bitmap[index] + ', ')
                    index += 1
                f.write("\n")
            
            # End of bitmap data
            f.write("};\n\n")
        
        # Create bitmap structure for each bitmap file
        f.write("/**\n"
                " * Section: Bitmap Structures\n"
                " */\n\n")
        for i, bmp in enumerate(bitmap_table):
            # Create and initialize a bitmap structure
            f.write("// " + bmp.capitalize() + " Bitmap Structure\n"
                    "const bitmap_t " + bmp + " =\n"
                    "{\n"
                    "    (const uint8_t *) &" + bmp + "_bitmap,\n"
                    "    BITMAP_" + str(i) + "_WIDTH,\n"
                    "    BITMAP_" + str(i) + "_HEIGHT\n"
                    "};\n\n")

def font_to_array(filename, size):
    """
    Generates an 'array' representation of the font
    which is used to generate source and header files for the bitmap.
    
    filename (str): The font file which we wish to generate code for
    size (int): The maximum height of the font
    
    returns (tuple): 
        A tuple containing font data.
        |
        |--> font_table (dict): A nested dictionary of glyph data
        |    |
        |    \--> char (str) : glyph data (dict) 
        |                      |--> 'Width'    : glyph width (int)
        |                      |--> 'Height'   : glyph height (int)
        |                      |--> 'Location' : glyph index in font array (int)
        |                      |--> 'Ascent'   : glyph ascent (int)
        |                      |--> 'Dummy'    : glyph data added to glyph width (int)
        |                      |--> 'Bitmap'   : glyph pixel array (list)
        |                      \--> 'String'   : glyph ascii representation (list)  
        |
        |--> font_height (int): The maximum height of the font glyphs
        |
        \--> font_descent (int): The maximum descent below the baseline
    """
    fnt = Font(filename, size)
    font_table = {}
    
    # Prepare characters for rendering
    elements = string.ascii_letters +  string.digits + string.punctuation
    elements.replace('//', '/')
    keys = [i for i in elements]
    keys.sort()
    
    # Get maximum font height and maximum descent
    font_parameters = fnt.text_dimensions(elements)
    font_height = font_parameters[1]
    font_descent = font_parameters[2]
    
    # Render and format characters
    glyph_location = 0
    glyph_size = 0
    
    for char in keys:
        # Render character
        glyph = fnt.glyph_for_character(char)
        ch = glyph.bitmap
        
        # Format character
        bitmap_data = tuple() 
        bitmap = [i for i in ch.pixels]        
        bitmap_data, dummy_added = add_dummy_data(bitmap, ch.width, ch.height)
        glyph_bitmap = format_bitmap(bitmap_data[0])
        # Represent the width of the bitmap as the SSD1322 does, where each
        # column address represents 4 pixels.This requires a division by 2 (each byte represents 2 pixels)
        # but we divide by 4 because format_bitmap() halves the width of the bitmap.
        glyph_width = bitmap_data[1] // 4
        glyph_height = bitmap_data[2]
        glyph_string = ch.__repr__()
        glyph_ascent = glyph.ascent
        
        font_table[char] = {
                            'Width'    : glyph_width,
                            'Height'   : glyph_height, 
                            'Location' : glyph_location,
                            'Ascent'   : glyph_ascent, 
                            'Dummy'    : dummy_added,
                            'Bitmap'   : glyph_bitmap,
                            'String'   : glyph_string.split('\n'),
                            }
        
        glyph_size = glyph_width * 2 * glyph_height
        # Update the location of the next character in the font array
        glyph_location += glyph_size
        
    return (font_table, font_height, font_descent)

def font_to_c(filename, size):
    """
    Generates font header (.h) and source (.c) files in the active directory
    
    filename (str): The filename of the font. The font has to be in the
                    active directory.
    size (int): The maximum height of the font in pixels
    
    Returns: None
    """
    # Remove file extension and leading digits
    file = filename.split('.')[0].lstrip(string.digits)
    # Replace punctuation characters with underscores
    t = {ord(i) : '_' for i in string.punctuation}
    file = file.translate(t)
    
    # Get font data
    font_table, font_height, font_descent = font_to_array(filename, size)  
    
    # Generate font header file
    with open(file + ".h", "w") as f:
        f.write("/**\n"
                " * @File Name\n"
                " *   " + file + ".h\n *\n"
                " * @Description\n"
                " *   This header file provides access to the installed font\n"
                " *   This code was auto generated with ssd13322_resource_utility.py\n"
                " */\n\n"
                "/**\n"
                " * Section: Included Files\n"
                " */\n\n"
                "#include \"ssd1322.h\"\n\n"
                "// Font Structure\n"
                "extern const font_t " + file + ";\n")
        
    # Generate font source file 
    with open(file + ".c", "w") as f:
        f.write("/**\n"
                " * @File Name\n"
                " *   " + file + ".c\n *\n"
                " * @Description\n"
                " *   This source file contains pixel data of the installed font\n"
                " *   This code was auto generated with ssd1322_resource_utility.py\n"
                " */\n\n"
                "/**\n"
                " * Section: Included Files\n"
                " */\n\n"
                "#include \"ssd1322.h\"\n\n"
                "/**\n"
                " * Section: Module Definitions\n"
                " */\n\n"
                "#define FONT_HEIGHT " + str(font_height) + "\n"
                "#define FONT_DESCENT " + str(font_descent) + "\n\n")
                
        # Write font table to file  
        f.write("/**\n"
                " * Section: Font Table\n"
                " */\n\n"
                "// Font table contains glyph metadata\n"
                "const font_table_entry_t " + file + "_font_table[] =\n{\n")
        
        keys = list(font_table.keys())
        for i in keys:
            # Convert font parameters to hexadecimal strings
            location = "0x%04X" % font_table[i]['Location']
            width    = "0x%02X" % font_table[i]['Width']
            height   = "0x%02X" % font_table[i]['Height']
            ascent   = "0x%02X" % font_table[i]['Ascent']
            dummy    = "0x%02X" % font_table[i]['Dummy']       
            # Make a font table entry for the current glyph
            f.write("    {%s, %s, %s, %s, %s},         " % \
                    (location, width, height, ascent, dummy))
            f.write("// Character - \"{}\", Ascii - {}\n".format(i, ord(i)))   
        # End of Font table
        f.write("};\n\n")   
            
        # Write font data
        f.write("/**\n"
                " * Section: Font Body\n"
                " */\n\n"
                "// Each byte represents two pixels\n"
                "const uint8_t " + file + "_font[] = \n{\n")
        
        for char in keys:
            # Reset index counter
            index = 0
            # Write one line of current glyph data
            f.write("    // " + 72 * "*" + "\n")
            f.write("    // * Character - \"{}\", Ascii - {}\n".format(char, ord(char)))
            f.write("    // " + 72 * "*" + "\n")
            for i in range(font_table[char]['Height']):
                f.write("    ")
                for j in range(font_table[char]['Width'] * 2):
                    data = font_table[char]['Bitmap'][index]
                    f.write(data + ', ')
                    index += 1
                # Write one line of the current glyph string representation
                f.write("         //  " + font_table[char]['String'][i])
                f.write("\n")
            # Move to next glyph
            f.write("\n")    
        # End of font array    
        f.write("};\n\n")    
            
        # Create and initialize font structure
        f.write("/**\n"
                " * Section: Font Structure\n"
                " */\n\n"
                "// Initialize font structure\n"
                "const font_t " + file + " = \n"
                "{\n"
                "    (const uint8_t *) &" + file + "_font,\n"
                "    (const font_table_entry_t *) &" + file + "_font_table,\n"
                "    FONT_HEIGHT,\n"
                "    FONT_DESCENT\n"
                "};\n")
            
if __name__ == "__main__":
    # Ensure that the .bmp and .ttf files you're trying to generate code for
    # are in the working directory.
    font_to_c("Lato-Regular.ttf", 27)
    bitmap_to_c(["ok.bmp", "CN.bmp"])
                