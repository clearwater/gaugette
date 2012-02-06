#!/usr/bin/env ruby
require 'rubygems'
require 'pp'
require 'cairo'

# Uses cairo ruby gem for graphics.
# I tried gd2, but it didn't seem to offer
# tools for antialiasing, line caps, etc
# and the results looked pretty crude.

# coordinate system:
# 0,0 is top left
# postive is right,down

# actual size is 26mm x 38mm
# hole at 8mm from bottom

SCALE = 2.0
WHITE = [1.0,1.0,1.0, 1]
BLACK = [0.0,0.0,0.0, 1]
IMAGE_DX = 380 * SCALE
IMAGE_DY = 260 * SCALE
CENTER_X = IMAGE_DX/2
CENTER_Y = (260-80) * SCALE

ANGLE_0 = -230.0/2.0
ANGLE_1 = 230.0/2.0


class Numeric
  def radians
    self * Math::PI / 180.0
  end
  def degrees
    self * 180.0 / Math::PI
  end
end

# returns [x,y] coordinate of point polar from cx,cy
def arc_coord(cx,cy,angle,radius)
  #PP.pp [angle,radius]
  #angle += 180.0
  [cx + Math.sin(angle.radians) * radius, cy - Math.cos(angle.radians) * radius]
end


def create_context
  surface = Cairo::ImageSurface.new(IMAGE_DX, IMAGE_DY)
  context = Cairo::Context.new(surface)
  context.set_source_rgba(WHITE)
  context.paint
  return context
end

def clip_outside(context, x0,y0,x1,y1)
  context.move_to(0,0)
  context.line_to(IMAGE_DX,0)
  context.line_to(IMAGE_DX,IMAGE_DY)
  context.line_to(0,IMAGE_DY)
  context.line_to(0,0)

  context.move_to(x0,y0)
  context.line_to(x0,y1)
  context.line_to(x1,y1)
  context.line_to(x1,y0)
  context.line_to(x0,y0)

  context.clip
end

def draw_tick(context, low, high, steps, except=nil)
  (0..steps).each do |step|
    if except.nil? || (step % except)>0
      angle = low + (high-low) * step/steps
      PP.pp [step, angle]
      x0,y0 = [CENTER_X,CENTER_Y]
      x1,y1 = arc_coord(CENTER_X,CENTER_Y,angle,IMAGE_DX)
      context.move_to(x0,y0)
      context.line_to(x1,y1)
      context.stroke
    end
  end
end

def draw_grid(context)
  minor_width = 2 * SCALE
  major_width = 4 * SCALE
  clip = 50 * SCALE
  # even clip on left, top, right, but bottom gets clipped differently
  clip_outside(context, clip,clip,IMAGE_DX-clip,IMAGE_DY-clip/2)
  context.set_line_cap(Cairo::LINE_CAP_ROUND)
  context.set_source_rgba(BLACK)
  context.set_line_width(major_width)
  steps = 10
  draw_tick(context, ANGLE_0,ANGLE_1, steps)
  
  clip = 30 * SCALE
  clip_outside(context, clip,clip,IMAGE_DX-clip,IMAGE_DY)
  context.set_line_width(minor_width)
  draw_tick(context, ANGLE_0, ANGLE_1, 50,5)

  context.reset_clip
end

def draw_arc(context)
  width = 2 * SCALE
  radius = 180
  angle_0 = ANGLE_0 - 90.0
  angle_1 = ANGLE_1 - 90.0
  context.save
  context.set_line_width(width)
  context.arc(CENTER_X, CENTER_Y, radius, angle_0.radians, angle_1.radians)
  context.stroke
  context.restore
end

def draw_labels(context)

  context.select_font_face("Arial", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD)
  context.set_font_size(30.0 * SCALE)

  steps = 10
  rad = 150 * SCALE
  (0..steps).each do |step|
    text = "%d" % [step * 10]
    angle = ANGLE_0 + (ANGLE_1-ANGLE_0) * step/steps
    x,y = arc_coord(CENTER_X,CENTER_Y,angle,rad)
    context.move_to(x,y)
    context.text_path(text)
    context.fill
    puts text
  end
end

def draw_center(context)
  context.save
  context.set_line_width(1)
  radius = 32/2 * SCALE
  context.arc(CENTER_X, CENTER_Y, radius, 0.0, 2 * Math::PI)
  context.stroke
  context.restore
end

def draw_box(context)
  context.save
  context.set_line_width(1)
  context.move_to(0,0)
  context.line_to(IMAGE_DX-1, 0)
  context.line_to(IMAGE_DX-1, IMAGE_DY-1)
  context.line_to(0, IMAGE_DY-1)
  context.line_to(0,0)
  context.stroke
  context.restore
end

context = create_context
draw_grid(context)
#draw_labels(context)
draw_center(context)
draw_arc(context)
draw_box(context)
context.target.write_to_png("dial.png")
