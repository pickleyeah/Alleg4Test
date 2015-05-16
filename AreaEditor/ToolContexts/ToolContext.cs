using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor.ToolContexts
{
    public abstract class ToolContext
    {
        public PropertyGrid EditorGrid { get; set; }
        public Control EditorSurface { get; set; }

        public ToolContext(PropertyGrid grid, Control surface)
        {
            EditorGrid = grid;
            EditorSurface = surface;
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
        public virtual void ImageDoubleClicked(string imageName, Image image, Area area)
        {
        }
        public virtual void KeyDown(object sender, KeyEventArgs e, Area m_area)
        {
        }
    }
}
