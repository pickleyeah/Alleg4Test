﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor.ToolContexts
{
    public class AddEntityToolContext : ToolContext
    {
        public AddEntityToolContext(PropertyGrid grid, ImageList imgList) :
            base(grid, imgList)
        {
        }
    }
}
