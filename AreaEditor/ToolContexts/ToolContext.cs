using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor.ToolContexts
{
    public abstract class ToolContext
    {
        public PropertyGrid EditorGrid { get; set; }
        public ImageList EditorImageList { get; set; }

        public ToolContext(PropertyGrid grid, ImageList imgList)
        {
            EditorGrid = grid;
            EditorImageList = imgList;
        }

        public virtual void MouseDown(object sender, MouseEventArgs e, Area area)
        {
        }
        public virtual void MouseUp(object sender, MouseEventArgs e, Area area)
        {
        }
        public virtual void MouseMove(object sender, MouseEventArgs e, Area area)
        {
        }
        public virtual void MouseEnter(object sender, EventArgs e, Area area)
        {
        }
        public virtual void MouseLeave(object sender, EventArgs e, Area area)
        {
        }
        public virtual void Paint(object sender, PaintEventArgs e, Area area)
        {
        }
    }
}
