<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:template match="/">
  <xsl:element name="html">
    <xsl:element name="head">
      <xsl:element name="title">Ramblings - <xsl:value-of select="commentary/meta/title" /> - <xsl:value-of select="commentary/meta/subtitle" /></xsl:element>
    </xsl:element>

    <xsl:element name="body">
      <xsl:element name="h1"><xsl:value-of select="commentary/meta/title" /></xsl:element>
      <xsl:element name="h3"><xsl:value-of select="commentary/meta/subtitle" /></xsl:element>
      By: <xsl:value-of select="commentary/meta/author/first_name" /> <xsl:value-of select="commentary/meta/author/last_name" /><xsl:element name="br" />
      Date: <xsl:value-of select="commentary/meta/date" /><xsl:element name="br" />

      <xsl:for-each select="./commentary/body">
        <xsl:apply-templates />
      </xsl:for-each>

    </xsl:element>
  </xsl:element>
</xsl:template>

<xsl:template match="para">
  <xsl:element name="p">
    <xsl:apply-templates />
  </xsl:element>
</xsl:template>

<xsl:template match="ol">
  <xsl:element name="ol">
    <xsl:apply-templates select="li" />
  </xsl:element>
</xsl:template>

<xsl:template match="li">
  <xsl:element name="li">
    <xsl:value-of select="." />
  </xsl:element>
</xsl:template>

<xsl:template match="thought">
  <xsl:element name="i">
    <xsl:value-of select="." />
  </xsl:element>
</xsl:template>
</xsl:stylesheet>
