using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor.ToolContexts
{
    public class PaintBlockOverlayToolContext : ToolContext
    {
        public PaintBlockOverlayToolContext(PropertyGrid grid, Control surface) :
            base(grid, surface)
        {
        }
    }
}
