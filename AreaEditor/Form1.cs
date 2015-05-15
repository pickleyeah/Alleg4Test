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
        
        private const int m_blockSize = 64;


        public Form1()
        {
            InitializeComponent();
            pictureBox1.MouseWheel += pictureBox1_MouseWheel;
            LoadImageMap();
        }

        private void RefreshDisplay()
        {
            pictureBox1.BeginInvoke(new MethodInvoker(() =>
                {
                    pictureBox1.Width = m_area.width * m_blockSize + 1;
                    pictureBox1.Height = m_area.height * m_blockSize + 1;
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

        void pictureBox1_MouseWheel(object sender, MouseEventArgs e)
        {
            
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

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            if (m_area == null)
                return;
            Graphics g = e.Graphics;
            for (int y = 0; y < m_area.height; y++)
            {
                for (int x = 0; x < m_area.width; x++)
                {
                    var imageName = m_area.GetBlock(x, y).sprite;
                    g.DrawImage(m_imageMap[imageName], new Point(x * m_blockSize, y * m_blockSize));
                }
            }

            // Draw cursor image
            if (m_displayCursorImage && !string.IsNullOrEmpty(m_cursorImage))
                g.DrawImage(imageList1.Images[m_cursorImage], m_cursorImagePos);

            Pen p = new Pen(Color.Black);

            for (int y = 0; y < pictureBox1.Height; y += m_blockSize)
                g.DrawLine(p, 0, y, pictureBox1.Width, y);
            for (int x = 0; x < pictureBox1.Width; x += m_blockSize)
                g.DrawLine(p, x, 0, x, pictureBox1.Height);
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (frmNewDialog dialog = new frmNewDialog())
            {
                if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                    CreateArea(dialog.AreaWidth, dialog.AreaHeight);
            }
        }

        private void pictureBox1_MouseClick(object sender, MouseEventArgs e)
        {
            if (m_area == null)
                return;
            int x = e.X / m_blockSize;
            int y = e.Y / m_blockSize;
            if (x >= m_area.width || y >= m_area.height)
                return;

            var block = m_area.GetBlock(x, y);
            if (m_cursorImage != null)
            {
                block.sprite = m_cursorImage;
            }
            propertyGrid1.SelectedObject = m_area.GetBlock(x, y);
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

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            m_cursorImagePos.X = e.X - (e.X % m_blockSize);
            m_cursorImagePos.Y = e.Y - (e.Y % m_blockSize);
            pictureBox1.Invalidate();
        }

        private void pictureBox1_MouseEnter(object sender, EventArgs e)
        {
            m_displayCursorImage = true;
        }

        private void pictureBox1_MouseLeave(object sender, EventArgs e)
        {
            m_displayCursorImage = false;
            pictureBox1.Invalidate();
        }
    }
}
