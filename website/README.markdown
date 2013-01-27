## Layouts
* base.html - A very basic layout that just defines boilerplate HTML and leaves the rest to be added as *content*.
* post.html - Extends from base.html by *including* a header, a sidebar, and a footer; and leaves the rest to be added as *content*.

## Includes
* header.html - This includes the logo and the title.
* sidebar.html - The contents of the sidebar.
* footer.html - A copyright notice.

## Posts
Posts use the post.html layout. Posts use the *none* permalink settings, so that they are referred only by their *title*.

* index.markdown - The main page.
* installation.markdown - Installation notes.
* license.markdown - Libxml-ruby's license.

Note: Emphasised words are Jekyll terminology.
