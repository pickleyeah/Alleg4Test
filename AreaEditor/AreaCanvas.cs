using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor
{
    public partial class AreaCanvas : Control
    {
        private Pen m_blackPen = new Pen(Color.Black);

        public bool DrawGrid { get; set; }

        private Area m_area;
        public Area CurrentArea
        {
            get { return m_area; }
            set
            {
                m_area = value;
                if (m_area != null)
                {
                    Width = m_area.width * Area.BlockSize + 1;
                    Height = m_area.height * Area.BlockSize + 1;
                }
            }
        }

        public AreaCanvas()
        {
            DoubleBuffered = true;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (m_area == null)
                return;
            base.OnPaint(e);
            // Draw grid
            var g = e.Graphics;
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            if (DrawGrid)
            {
                for (int y = 0; y < Width; y += Area.BlockSize)
                    g.DrawLine(m_blackPen, 0, y, Width, y);
                for (int x = 0; x < Height; x += Area.BlockSize)
                    g.DrawLine(m_blackPen, x, 0, x, Height);
            }
            
        }
    }
}
