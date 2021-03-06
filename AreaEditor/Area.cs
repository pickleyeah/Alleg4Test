﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AreaEditor
{
    public class Area
    {
        public const int BlockSize = 64;

        public Area() { }
        public Area(int width_, int height_)
        {
            width = width_;
            height = height_;
            startPos = new Vec2();
            Blocks = new Block[width_ * height_];
            for (int i = 0; i < Blocks.Length; i++)
                Blocks[i] = new Block();
            Entities = new List<Entity>();
        }
        [XmlAttribute, ReadOnly(true)]
        public int width { get; set; }
        [XmlAttribute, ReadOnly(true)]
        public int height { get; set; }

        public Vec2 startPos { get; set; }
        public int startDir { get; set; }

        [Browsable(false)]
        public Block[] Blocks { get; set; }

        public Block GetBlock(int x, int y)
        {
            return Blocks[y * width + x];
        }
        [Browsable(false)]
        public List<Entity> Entities { get; set; }
    }

    public class Block
    {
        public Block()
        {
            sprite = "Data/Tiles/Grass.png";
        }

        [XmlAttribute("flags"), Browsable(false)]
        public int _flags { get; set; }

        [Editor(typeof(Utils.FlagEnumUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [XmlIgnore]
        public BlockFlags Flags
        {
            get
            {
                return (BlockFlags)_flags;
            }
            set
            {
                _flags = (int)value;
            }
        }

        // For storing a pointer to the sprite used
        [Browsable(false), XmlIgnore]
        public Image CachedBlockImage { get; set; }

        private string _sprite;

        [XmlAttribute]
        [ReadOnly(true)]
        public string sprite
        {
            get { return _sprite; }
            set
            {
                _sprite = value;
                CachedBlockImage = null;
            }
        }

        public Overlay Overlay { get; set; }

        //public string Overlay

        [DefaultValue(null)]
        public WarpDetails Warp { get; set; }
    }

    public class Overlay
    {
        private string _sprite;

        [Browsable(false), XmlIgnore]
        public Image CachedBlockImage { get; set; }

        [XmlAttribute]
        [ReadOnly(true)]
        public string sprite
        {
            get { return _sprite; }
            set
            {
                _sprite = value;
                CachedBlockImage = null;
            }
        }
    }

    [Flags]
    public enum BlockFlags
    {
        None = 0,
        COLLIDE_NORTH = 0x01,
        COLLIDE_WEST = 0x02,
        COLLIDE_SOUTH = 0x04,
        COLLIDE_EAST = 0x08,
        COLLIDE_ALL = 0x0f,

    };

    public class WarpDetailsConverter : ExpandableObjectConverter
    {
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(WarpDetails))
                return true;
            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
        {
            return base.ConvertTo(context, culture, value, destinationType);
        }
    }

    public enum DIR
    {
        DIR_NORTH,
        DIR_EAST,
        DIR_SOUTH,
        DIR_WEST,
    }

    [TypeConverterAttribute(typeof(WarpDetailsConverter))]
    public class WarpDetails
    {
        public string Area { get; set; }
        public Vec2 Pos { get; set; }

        [XmlElement("Dir"), Browsable(false)]
        public int _dir { get; set; }
        [XmlIgnore]
        public DIR Dir
        {
            get
            {
                return (DIR)_dir;
            }
            set
            {
                _dir = (int)value;
            }
        }
    }
}
