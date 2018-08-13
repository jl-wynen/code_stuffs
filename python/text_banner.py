"""
Paint some text in an Axes' background.
"""

import math

import matplotlib.pyplot as plt
from matplotlib.transforms import IdentityTransform, Affine2D
from matplotlib.textpath import TextPath
from matplotlib.patches import PathPatch
from matplotlib.offsetbox import AnnotationBbox, AuxTransformBox

import numpy as np


def rotated_scale(width, height, angle, target_width, target_height):
    """Compute scale factors for rotated rectangle."""
    # dimensions of rotated width
    wxp = np.abs(width*np.cos(angle))
    wyp = np.abs(width*np.sin(angle))
    # dimensions of rotated height
    hxp = np.abs(height*np.sin(angle))
    hyp = np.abs(height*np.cos(angle))
    # get scales
    return target_width/(wxp+hxp), target_height/(wyp+hyp)


def text_banner(axes, text, facecolor="red", edgecolor="darkred", linewidth=1,
                alpha=0.3, angleadjust=True, zorder=0):
    """
    Paint text across a hole axes.
    For height > width, angleadjust should be False.
    """

    # draw the text into a patch
    textpath = TextPath((0, 0), text, size=20, transform=axes.transAxes)
    tp_bbox = textpath.get_extents()
    patch = PathPatch(textpath, fc=facecolor, ec=edgecolor, lw=linewidth, alpha=alpha,
                      transform=IdentityTransform(), zorder=11)

    # get angle and scale to transform text to axes coordinates
    ax_bbox = axes.get_window_extent()
    angle = math.atan2(ax_bbox.height, ax_bbox.width) * \
            (ax_bbox.height/ax_bbox.width if angleadjust else 1)
    scale = min(*rotated_scale(tp_bbox.width, tp_bbox.height, angle,
                               ax_bbox.width, ax_bbox.height))*0.95

    # paint the patch into the axes
    offsetbox = AuxTransformBox(Affine2D().rotate(angle).scale(scale))
    offsetbox.add_artist(patch)
    artist = AnnotationBbox(offsetbox, (0.5, 0.5),
                            xycoords='axes fraction',
                            frameon=False)
    artist.set_zorder(zorder)
    axes.add_artist(artist)


# showcase it
fig = plt.figure(figsize=(11,5))
ax = fig.add_subplot(121)

hist, bin_edges = np.histogram(np.random.randn(1000), 20)
ax.plot((bin_edges[:-1]+bin_edges[1:])/2, hist)
plt.tight_layout()
text_banner(ax, "PRELIMINARY", angleadjust=False)

ax = fig.add_subplot(122)
hist, bin_edges = np.histogram(np.random.randn(1000), 20)
ax.plot((bin_edges[:-1]+bin_edges[1:])/2, hist)
plt.tight_layout()
text_banner(ax, "So Solls Sein!", angleadjust=False,
            facecolor="blue", edgecolor="none")

plt.show()
