using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor.ToolContexts
{
    class PanToolContext : ToolContext
    {
        private bool m_panning = false;
        private Point m_lastMousePos;

        public PanToolContext(PropertyGrid grid, Control surface) :
            base(grid, surface)
        {
        }

        public override void MouseDown(object sender, MouseEventArgs e, Area area)
        {
            m_panning = true;
        }

        public override void MouseMove(object sender, MouseEventArgs e, Area area)
        {
            if (m_panning)
            {
                // TODO: Figure out scrolling
            }
            m_lastMousePos = e.Location;
        }

        public override void MouseUp(object sender, MouseEventArgs e, Area area)
        {
            m_panning = false;
        }
    }
}
