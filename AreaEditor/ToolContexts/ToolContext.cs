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

        public ToolContext(PropertyGrid grid)
        {
            EditorGrid = grid;
        }
    }
}
