using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml.Serialization;
using System.IO;
using AreaEditor.ToolContexts;

namespace AreaEditor
{
    public partial class Form1 : Form
    {
        private Area m_area;
        private string m_cursorImage;
        private bool m_displayCursorImage;
        private Point m_cursorImagePos = new Point(0, 0);

        private string m_filename;
        private Dictionary<string, Image> m_imageMap = new Dictionary<string, Image>();
        private List<ToolContext> m_toolContexts = new List<ToolContext>();
        private ToolContext m_currentTool;
        

        public Form1()
        {
            InitializeComponent();
            pictureBox1.MouseWheel += pictureBox1_MouseWheel;
            InitToolContexts();
            LoadImageMap();
        }

        private void InitToolContexts()
        {
            m_toolContexts.Clear();
            m_toolContexts.Add(new SelectBlockToolContext(propertyGrid1, imageList1));

            m_currentTool = m_toolContexts.Where(t => t is SelectBlockToolContext).FirstOrDefault();
        }

        private void RefreshDisplay()
        {
            pictureBox1.BeginInvoke(new MethodInvoker(() =>
                {
                    pictureBox1.Width = m_area.width * Area.BlockSize + 1;
                    pictureBox1.Height = m_area.height * Area.BlockSize + 1;
                    pictureBox1.Invalidate();
                }));
        }

        private void LoadImageMap()
        {
            Directory.CreateDirectory("Data/Tiles");
            foreach (var path in Directory.EnumerateFiles("Data/Tiles", "*.png"))
            {
                string f = path.Replace('\\', '/');   // Get rid of backslashes to make paths consistent between game/editor
                Image image = Image.FromFile(f);
                ListViewItem item = new ListViewItem();
                item.Text = f;
                item.ImageKey = f;
                listViewTiles.Items.Add(item);
                imageList1.Images.Add(f, image);
                m_imageMap.Add(f, image);
            }
        }

        private void CreateArea(int width, int height)
        {
            m_area = new Area(width, height);
            m_filename = null;
            RefreshDisplay();
        }

        private bool LoadArea(string filename)
        {
            try
            {
                XmlSerializer serializer = new XmlSerializer(typeof(Area));
                m_area = serializer.Deserialize(new StreamReader(filename)) as Area;
                if (m_area == null)
                    return false;
                RefreshDisplay();
            }
            catch (Exception)
            {
                return false;
            }
            return true;
        }

        private bool SaveArea(string filename)
        {
            try
            {
                XmlSerializer serializer = new XmlSerializer(typeof(Area));
                using (var writer = new StreamWriter(filename))
                    serializer.Serialize(writer, m_area);
                return true;
            }
            catch
            {
                return false;
            }
        }

        private void splitContainer1_Panel1_MouseClick(object sender, MouseEventArgs e)
        {
            propertyGrid1.SelectedObject = m_area;
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (frmNewDialog dialog = new frmNewDialog())
            {
                if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                    CreateArea(dialog.AreaWidth, dialog.AreaHeight);
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog fileDialog = new OpenFileDialog())
            {
                fileDialog.Filter = "XML File (*.xml)|*.xml|All Files (*.*)|*.*";
                if (DialogResult.OK == fileDialog.ShowDialog())
                {
                    if (!LoadArea(fileDialog.FileName))
                    {
                        MessageBox.Show("Unable to open file " + fileDialog.FileName);
                        return;
                    }
                    m_filename = fileDialog.FileName;
                }
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(m_filename))
            {
                saveAsToolStripMenuItem_Click(sender, e);
                return;
            }

            if (!SaveArea(m_filename))
            {
                MessageBox.Show("Unable to save file " + m_filename);
                return;
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog fileDialog = new SaveFileDialog())
            {
                fileDialog.Filter = "XML File (*.xml)|*.xml|All Files (*.*)|*.*";
                if (DialogResult.OK == fileDialog.ShowDialog())
                {
                    if (!SaveArea(fileDialog.FileName))
                    {
                        MessageBox.Show("Unable to save file " + fileDialog.FileName);
                        return;
                    }
                    m_filename = fileDialog.FileName;
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void listViewTiles_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewTiles.SelectedItems.Count == 1)
            {
                var item = listViewTiles.SelectedItems[0];
                m_cursorImage = item.ImageKey;
            }
        }

        #region PictureBox Events
        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (m_area == null)
                return;
            //m_cursorImagePos.X = e.X - (e.X % m_blockSize);
            //m_cursorImagePos.Y = e.Y - (e.Y % m_blockSize);
            m_currentTool.MouseMove(sender, e, m_area);
            pictureBox1.Invalidate();
        }

        private void pictureBox1_MouseEnter(object sender, EventArgs e)
        {
            if (m_area == null)
                return;
            //m_displayCursorImage = true;
            m_currentTool.MouseEnter(sender, e, m_area);
            pictureBox1.Invalidate();
        }

        private void pictureBox1_MouseLeave(object sender, EventArgs e)
        {
            if (m_area == null)
                return;
            //m_displayCursorImage = false;
            m_currentTool.MouseLeave(sender, e, m_area);
            pictureBox1.Invalidate();
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (m_area == null)
                return;
            m_currentTool.MouseDown(sender, e, m_area);
            pictureBox1.Invalidate();
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (m_area == null)
                return;
            m_currentTool.MouseUp(sender, e, m_area);
            pictureBox1.Invalidate();
        }

        void pictureBox1_MouseWheel(object sender, MouseEventArgs e)
        {

        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            if (m_area == null)
                return;
            Graphics g = e.Graphics;
            // Draw Blocks
            for (int y = 0; y < m_area.height; y++)
            {
                for (int x = 0; x < m_area.width; x++)
                {
                    var imageName = m_area.GetBlock(x, y).sprite;
                    g.DrawImage(imageList1.Images[imageName], new Point(x * Area.BlockSize, y * Area.BlockSize));
                }
            }
            // TODO: Draw entities

            // Draw tool context data
            m_currentTool.Paint(sender, e, m_area);

            // Draw cursor image
            //if (m_displayCursorImage && !string.IsNullOrEmpty(m_cursorImage))
            //    g.DrawImage(imageList1.Images[m_cursorImage], m_cursorImagePos);

            // Draw grid
            Pen p = new Pen(Color.Black);
            for (int y = 0; y < pictureBox1.Height; y += Area.BlockSize)
                g.DrawLine(p, 0, y, pictureBox1.Width, y);
            for (int x = 0; x < pictureBox1.Width; x += Area.BlockSize)
                g.DrawLine(p, x, 0, x, pictureBox1.Height);
        }
        #endregion
    }
}
