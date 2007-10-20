<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:output cdata-section-elements="script"/>

  <xsl:template match="/">

    <html>
      <head>
        <title>Libxml</title>
        <link rel="stylesheet" href="styles/normal.css" />
        <link REL='SHORTCUT ICON' HREF="images/xml-ruby.png" />
      </head>
      <body>
      <div class="container">
        
        <table>
          <tr valign="top">
            <td class='navlinks'>
              <img src="images/red-cube.jpg" align="top" style="margin-left: -100px;"/>

              <br/><br/>

              <strong>Navigation</strong><br/>
              <li><a href="index.xml">Home</a></li>
              <li><a href="install.xml">Installation</a></li>
              <li><a href="rdoc/">Documentation</a></li>
              <li><a href="http://rubyforge.org/projects/libxml">Project Page</a></li>
              <li><a href="license.xml">License</a></li>
              <br/>
              <strong>External</strong><br/>
              <li><a href="http://xmlsoft.org/">Libxml2 project</a></li>
            </td>
            <td style="padding: 10px;">
              <h1><span style="color: red;">Libxml</span>-Ruby Project</h1>

              <xsl:apply-templates />
            </td>
          </tr>
        </table>

        <div class='copyright'>
          Copyright &#x00A9; 2001-2006 Libxml-Ruby project contributors.<br/>
          Website is, yea baby!, pure XML/XSLT<br/>
        </div>

      </div>
      </body>
    </html>

  </xsl:template>

  <xsl:template match="content">
    <xsl:copy-of select="."/>
  </xsl:template>

</xsl:stylesheet>

