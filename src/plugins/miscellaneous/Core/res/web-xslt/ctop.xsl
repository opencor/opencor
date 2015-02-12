<xsl:stylesheet
  version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns="http://www.w3.org/1998/Math/MathML"
>

<!--

Copyright David Carlisle 2001, 2002, 2008, 2009, 2013.

Use and distribution of this code are permitted under the terms of the <a
href="http://www.w3.org/Consortium/Legal/copyright-software-19980720"
>W3C Software Notice and License</a>.
Or the Apache 2, MIT or MPL 1.1 or MPL 2.0 licences.
2001-2002 MathML2 version
2008-2009     Updates for MathML3
-->

<xsl:output method="xml" />

<xsl:template match="/">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="*">
<xsl:copy>
  <xsl:copy-of select="@*"/>
  <xsl:apply-templates/>
</xsl:copy>
</xsl:template>


<!-- 4.4.1.1 cn -->

<xsl:template match="cn">
 <mn><xsl:apply-templates/></mn>
</xsl:template>

<xsl:template match="cn[@type='complex-cartesian']">
  <mrow>
    <mn><xsl:apply-templates select="text()[1]"/></mn>
    <mo>+</mo>
    <mn><xsl:apply-templates select="text()[2]"/></mn>
    <mo>&#8290;<!--invisible times--></mo>
    <mi>i<!-- imaginary i --></mi>
  </mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='complex_cartesian']]">
  <mrow>
    <mn><xsl:apply-templates select="*[2]"/></mn>
    <mo>+</mo>
    <mn><xsl:apply-templates select="*[3]"/></mn>
    <mo>&#8290;<!--invisible times--></mo>
    <mi>i<!-- imaginary i --></mi>
  </mrow>
</xsl:template>

<xsl:template match="cn[@type='rational']">
  <mrow>
    <mn><xsl:apply-templates select="text()[1]"/></mn>
    <mo>/</mo>
    <mn><xsl:apply-templates select="text()[2]"/></mn>
  </mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='rational']]">
  <mrow>
    <mn><xsl:apply-templates select="*[2]"/></mn>
    <mo>/</mo>
    <mn><xsl:apply-templates select="*[3]"/></mn>
  </mrow>
</xsl:template>

<xsl:template match="cn[not(@type) or @type='integer']">
  <xsl:choose>
  <xsl:when test="not(@base) or @base=10">
       <mn><xsl:apply-templates/></mn>
  </xsl:when>
  <xsl:otherwise>
  <msub>
    <mn><xsl:apply-templates/></mn>
    <mn><xsl:value-of select="@base"/></mn>
  </msub>
  </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="cn[@type='complex-polar']">
  <mrow>
    <mn><xsl:apply-templates select="text()[1]"/></mn>
    <mo>&#8290;<!--invisible times--></mo>
    <msup>
    <mi>e<!-- exponential e--></mi>
    <mrow>
     <mi>i<!-- imaginary i--></mi>
     <mo>&#8290;<!--invisible times--></mo>
     <mn><xsl:apply-templates select="text()[2]"/></mn>
    </mrow>
    </msup>
  </mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='complex_polar']]">
  <mrow>
    <xsl:apply-templates select="*[2]"/>
    <mo>&#8290;<!--invisible times--></mo>
    <msup>
    <mi>e<!-- exponential e--></mi>
    <mrow>
     <mi>i<!-- imaginary i--></mi>
     <mo>&#8290;<!--invisible times--></mo>
     <xsl:apply-templates select="*[3]"/>
    </mrow>
    </msup>
  </mrow>
</xsl:template>

<xsl:template match="cn[@type='e-notation']">
  <mn>
    <xsl:apply-templates select="sep/preceding-sibling::node()"/>
    <xsl:text>E</xsl:text>
    <xsl:apply-templates select="sep/following-sibling::node()"/>
  </mn>
</xsl:template>

<xsl:template match="cn[@type='hexdouble']">
  <mn>
    <xsl:text>0x</xsl:text>
    <xsl:apply-templates/>
  </mn>
</xsl:template>

<!-- 4.4.1.1 ci -->

<xsl:template match="ci/text()">
 <mi mathvariant="italic"><xsl:value-of select="."/></mi>
</xsl:template>

<xsl:template match="ci">
 <mrow><xsl:apply-templates/></mrow>
</xsl:template>

<!-- 4.4.1.2 csymbol -->

<xsl:template match="csymbol/text()">
 <mi><xsl:value-of select="."/></mi><!-- Robin Green r.d.greenATlancaster.ac.uk, Christoph Lange langecATweb.de-->
</xsl:template>

<xsl:template match="csymbol">
 <mrow><xsl:apply-templates/></mrow>
</xsl:template>

<!-- 4.4.2.1 apply 4.4.2.2 reln -->

<xsl:template match="apply|reln">
 <mrow>
   <xsl:choose>
     <xsl:when test="*[1]/*/*">
       <mfenced separators="">
         <xsl:apply-templates select="*[1]">
           <xsl:with-param name="p" select="10"/>
         </xsl:apply-templates>
       </mfenced>
     </xsl:when>
     <xsl:otherwise>
       <xsl:apply-templates select="*[1]">
         <xsl:with-param name="p" select="10"/>
       </xsl:apply-templates>
     </xsl:otherwise>
   </xsl:choose>
 <mo>&#8289;<!--function application--></mo>
 <mfenced open="(" close=")" separators=",">
 <xsl:apply-templates select="*[position()>1]"/>
 </mfenced>
 </mrow>
</xsl:template>


<xsl:template match="bind">
 <mrow>
   <xsl:choose>
     <xsl:when test="*[1]/*/*">
       <mfenced separators="">
         <xsl:apply-templates select="*[1]">
           <xsl:with-param name="p" select="10"/>
         </xsl:apply-templates>
       </mfenced>
     </xsl:when>
     <xsl:otherwise>
       <xsl:apply-templates select="*[1]">
         <xsl:with-param name="p" select="10"/>
       </xsl:apply-templates>
     </xsl:otherwise>
   </xsl:choose>
   <xsl:apply-templates select="bvar/*"/>
   <mo>.</mo>
   <xsl:apply-templates select="*[position()>1][not(self::bvar)]"/>
 </mrow>
</xsl:template>

<!-- 4.4.2.3 fn -->
<xsl:template match="fn">
 <mrow><xsl:apply-templates/></mrow>
</xsl:template>

<!-- 4.4.2.4 interval -->
<xsl:template match="interval[*[2]]">
 <mfenced open="[" close="]"><xsl:apply-templates/></mfenced>
</xsl:template>
<xsl:template match="interval[*[2]][@closure='open']" priority="2">
 <mfenced open="(" close=")"><xsl:apply-templates/></mfenced>
</xsl:template>
<xsl:template match="interval[*[2]][@closure='open-closed']" priority="2">
 <mfenced open="(" close="]"><xsl:apply-templates/></mfenced>
</xsl:template>
<xsl:template match="interval[*[2]][@closure='closed-open']" priority="2">
 <mfenced open="[" close=")"><xsl:apply-templates/></mfenced>
</xsl:template>

<xsl:template match="interval">
 <mfenced open="{{" close="}}"><xsl:apply-templates/></mfenced>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='integer_interval']]">
 <mfenced open="[" close="]"><xsl:apply-templates select="*[position()!=1]"/></mfenced>
</xsl:template>
<xsl:template match="apply[*[1][self::csymbol='interval']]">
 <mfenced open="[" close="]"><xsl:apply-templates select="*[position()!=1]"/></mfenced>
</xsl:template>
<xsl:template match="apply[*[1][self::csymbol='interval-cc']]">
 <mfenced open="[" close="]"><xsl:apply-templates select="*[position()!=1]"/></mfenced>
</xsl:template>
<xsl:template match="apply[*[1][self::csymbol='interval-oo']]">
 <mfenced open="(" close=")"><xsl:apply-templates select="*[position()!=1]"/></mfenced>
</xsl:template>
<xsl:template match="apply[*[1][self::csymbol='oriented_interval']]">
 <mfenced open="(" close=")"><xsl:apply-templates select="*[position()!=1]"/></mfenced>
</xsl:template>

<!-- 4.4.2.5 inverse -->

<xsl:template match="apply[*[1][self::inverse]]
                       |apply[*[1][self::csymbol='inverse']]">
 <msup>
  <xsl:apply-templates select="*[2]"/>
  <mrow><mo>(</mo><mn>-1</mn><mo>)</mo></mrow>
 </msup>
</xsl:template>

<!-- 4.4.2.6 sep -->

<!-- 4.4.2.7 condition -->
<xsl:template match="condition">
 <mrow><xsl:apply-templates/></mrow>
</xsl:template>

<!-- 4.4.2.8 declare -->
<xsl:template match="declare"/>

<!-- 4.4.2.9 lambda -->
<xsl:template match="lambda
                                |apply[*[1][self::csymbol='lambda']]
                                |bind[*[1][self::csymbol='lambda']]"><!--dpc-->
 <mrow>
  <mi>&#955;<!--lambda--></mi>
  <mrow>
   <xsl:choose>
    <xsl:when test="condition">
     <xsl:apply-templates select="condition/*"/>
    </xsl:when>
    <xsl:otherwise>
     <xsl:apply-templates select="bvar/*"/>
    </xsl:otherwise>
   </xsl:choose>
   <xsl:if test="domainofapplication">
    <mo>&#x2208;</mo>
    <xsl:apply-templates select="domainofapplication/*"/>
   </xsl:if>
  </mrow>
  <mo>.</mo>
  <mfenced>
   <xsl:apply-templates select="*[last()]"/>
  </mfenced>
 </mrow>
</xsl:template>

<xsl:template match="lambda[not(bvar)]" priority="2">
 <mrow>
  <xsl:apply-templates select="*[last()]"/>
  <msub>
   <mo>|</mo>
   <mrow>
    <xsl:apply-templates select="condition|interval|domainofapplication/*"/>
   </mrow>
  </msub>
 </mrow>
</xsl:template>


<!-- 4.4.2.10 compose -->
<xsl:template match="apply[*[1][self::compose]]
                       |apply[*[1][self::csymbol='left_compose']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="1"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8728;<!-- o --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>


<!-- 4.4.2.11` ident -->
<xsl:template match="ident">
<mi>id</mi>
</xsl:template>

<!-- 4.4.2.12` domain -->
<xsl:template match="domain">
<mi>domain</mi>
</xsl:template>

<!-- 4.4.2.13` codomain -->
<xsl:template match="codomain">
<mi>codomain</mi>
</xsl:template>

<!-- 4.4.2.14` image -->
<xsl:template match="image">
<mi>image</mi>
</xsl:template>

<!-- 4.4.2.15` domainofapplication -->
<xsl:template match="domainofapplication">
 <merror><mtext>unexpected domainofapplication</mtext></merror>
</xsl:template>

<xsl:template match="apply[*[2][self::bvar]][domainofapplication]" priority="0.4">
 <mrow>
  <munder>
   <xsl:apply-templates select="*[1]"/>
   <mrow>
    <xsl:apply-templates select="bvar/*"/>
    <mo>&#8712;<!-- in --></mo>
    <xsl:apply-templates select="domainofapplication/*"/>
   </mrow>
  </munder>
  <mfenced>
   <xsl:apply-templates select="domainofapplication/following-sibling::*"/>
  </mfenced>
 </mrow>
</xsl:template>

<xsl:template match="apply[domainofapplication]" priority="0.3">
 <mrow>
  <mrow><mi>restriction</mi>
  <mfenced>
   <xsl:apply-templates select="*[1]"/>
   <xsl:apply-templates select="domainofapplication/*"/>
  </mfenced>
  </mrow>
  <mfenced>
   <xsl:apply-templates select="domainofapplication/following-sibling::*"/>
  </mfenced>
 </mrow>
</xsl:template>

<!-- 4.4.2.16` piecewise -->
<xsl:template match="piecewise">
  <mrow>
    <mo>{</mo>
    <mtable>
      <xsl:for-each select="piece|otherwise">
        <mtr>
          <mtd><xsl:apply-templates select="*[1]"/></mtd>
          <xsl:choose><!--dpc-->
            <xsl:when test="self::piece">
              <mtd columnalign="left"><mtext>&#160; if &#160;</mtext></mtd>
              <mtd><xsl:apply-templates select="*[2]"/></mtd>
            </xsl:when>
            <xsl:otherwise>
              <mtd columnspan="2" columnalign="left"><mtext>&#160; otherwise</mtext></mtd>
            </xsl:otherwise>
          </xsl:choose>
        </mtr>
      </xsl:for-each>
    </mtable>
  </mrow>
</xsl:template>


<!-- 4.4.3.1 quotient -->
<xsl:template match="apply[*[1][self::quotient]]
                       |apply[*[1][self::csymbol='quotient']]">
<mrow>
<mo>&#8970;<!-- lfloor--></mo>
<xsl:apply-templates select="*[2]"/>
<mo>/</mo>
<xsl:apply-templates select="*[3]"/>
<mo>&#8971;<!-- rfloor--></mo>
</mrow>
</xsl:template>



<!-- 4.4.3.2 factorial -->
<xsl:template match="apply[*[1][self::factorial]]
                                |apply[*[1][self::csymbol='factorial']]">
<mrow>
<xsl:apply-templates select="*[2]">
  <xsl:with-param name="p" select="7"/>
</xsl:apply-templates>
<mo>!</mo>
</mrow>
</xsl:template>


<!-- 4.4.3.3 divide -->
<xsl:template match="apply[*[1][self::divide]]
                                |apply[*[1][self::csymbol='divide']]">
<mfrac>
  <mrow>
    <xsl:apply-templates select="*[position()!=last()]"/>
  </mrow>
  <mrow>
    <xsl:apply-templates select="*[last()]"/>
  </mrow>
</mfrac>
</xsl:template>


<!-- 4.4.3.4 max min -->
<xsl:template match="apply[*[1][self::max]]
                                |apply[*[1][self::csymbol='max']]">
<mrow>
  <mi>max</mi>
  <xsl:call-template name="set"/>
</mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::min]]|reln[*[1][self::min]]">
<mrow>
  <mi>min</mi><!--dpc-->
  <xsl:call-template name="set"/>
</mrow>
</xsl:template>

<!-- 4.4.3.5 minus -->
<xsl:template match="apply[*[1][self::minus] and count(*)=2]
                                |apply[*[1][self::csymbol='unary_minus']]">
<mrow>
  <mo>&#8722;<!--minus--></mo>
  <xsl:apply-templates select="*[2]">
      <xsl:with-param name="p" select="5"/>
  </xsl:apply-templates>
</mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::minus] and count(*)&gt;2]
                                |apply[*[1][self::csymbol='minus']]">
  <xsl:param name="p" select="0"/>
<xsl:call-template name="binary">
  <xsl:with-param name="mo"><mo>&#8722;<!--minus--></mo></xsl:with-param>
  <xsl:with-param name="p" select="$p"/>
  <xsl:with-param name="this-p" select="2"/>
</xsl:call-template>
</xsl:template>

<!-- 4.4.3.6 plus -->
<!-- put out - on leading term (mathjax report)-->
<xsl:template match="apply[*[1][self::plus]]
                                |apply[*[1][self::csymbol='plus']]">
  <xsl:param name="p" select="0"/>
  <mrow>
  <xsl:if test="$p &gt; 2"><mo>(</mo></xsl:if>
  <xsl:for-each select="*[position()&gt;1]">
   <xsl:choose>
    <xsl:when test="self::apply[*[1][self::times] and *[2][self::apply/*[1][self::minus] or
                                  self::cn[not(sep) and (number(.) &lt; 0)]]] or
                    self::apply[count(*)=2 and *[1][self::minus]] or
                    self::cn[not(sep) and (number(.) &lt; 0)]">
     <mo>&#8722;<!--minus--></mo>
    </xsl:when>
    <xsl:when test="position()!=1">
     <mo>+</mo>
    </xsl:when>
   </xsl:choose>
   <xsl:choose>
    <xsl:when test="self::cn[not(sep) and (number(.) &lt; 0)]">
     <mn><xsl:value-of select="-(.)"/></mn>
    </xsl:when>
    <xsl:when test=" self::apply[count(*)=2 and *[1][self::minus]]">
     <xsl:apply-templates select="*[2]">
      <xsl:with-param name="first" select="2"/>
      <xsl:with-param name="p" select="2"/>
     </xsl:apply-templates>
    </xsl:when>
    <xsl:when test="self::apply[*[1][self::times] and
                    *[2][self::cn[not(sep) and (number(.) &lt;0)]]]">
     <mrow>
      <mn><xsl:value-of select="-(*[2])"/></mn>
      <mo>&#8290;<!--invisible times--></mo>
      <xsl:apply-templates select=".">
       <xsl:with-param name="first" select="2"/>
       <xsl:with-param name="p" select="2"/>
      </xsl:apply-templates>
     </mrow>
    </xsl:when>
    <xsl:when test="self::apply[*[1][self::times] and
                    *[2][self::apply/*[1][self::minus]]]">
     <mrow>
      <xsl:apply-templates select="./*[2]/*[2]"/>
      <xsl:apply-templates select=".">
       <xsl:with-param name="first" select="2"/>
       <xsl:with-param name="p" select="2"/>
      </xsl:apply-templates>
     </mrow>
    </xsl:when>
    <xsl:otherwise>
     <xsl:apply-templates select=".">
      <xsl:with-param name="p" select="2"/>
     </xsl:apply-templates>
    </xsl:otherwise>
   </xsl:choose>
  </xsl:for-each>
  <xsl:if test="$p &gt; 2"><mo>)</mo></xsl:if>
  </mrow>
</xsl:template>


<!-- 4.4.3.7 power -->
<xsl:template match="apply[*[1][self::power]]
                                |apply[*[1][self::csymbol='power']]">
<xsl:param name="p" select="0"/>
<xsl:choose>
 <xsl:when test="$p&gt;=5">
<mrow>
<mo>(</mo>
<msup>
<xsl:apply-templates select="*[2]">
  <xsl:with-param name="p" select="5"/>
</xsl:apply-templates>
<xsl:apply-templates select="*[3]">
  <xsl:with-param name="p" select="5"/>
</xsl:apply-templates>
</msup>
<mo>)</mo>
</mrow>
 </xsl:when>
<xsl:otherwise>
<msup>
<xsl:apply-templates select="*[2]">
  <xsl:with-param name="p" select="5"/>
</xsl:apply-templates>
<xsl:apply-templates select="*[3]">
  <xsl:with-param name="p" select="5"/>
</xsl:apply-templates>
</msup>
</xsl:otherwise>
</xsl:choose>
</xsl:template>

<!-- 4.4.3.8 remainder -->
<xsl:template match="apply[*[1][self::rem]]
                       |apply[*[1][self::csymbol='rem']]">
  <xsl:param name="p" select="0"/>
<xsl:call-template name="binary">
  <xsl:with-param name="mo"><mo>mod</mo></xsl:with-param>
  <xsl:with-param name="p" select="$p"/>
  <xsl:with-param name="this-p" select="3"/>
</xsl:call-template>
</xsl:template>

<!-- 4.4.3.9 times -->
<xsl:template match="apply[*[1][self::times]]
                                |apply[*[1][self::csymbol='times']]"
              name="times">
  <xsl:param name="p" select="0"/>
  <xsl:param name="first" select="1"/>
  <mrow>
  <xsl:if test="$p &gt; 3"><mo>(</mo></xsl:if>
  <xsl:for-each select="*[position()&gt;1]">
   <xsl:if test="position() &gt; 1">
    <mo>&#183;<!-- middle dot --></mo>
   </xsl:if>
   <xsl:if test="position()&gt;= $first">
   <xsl:apply-templates select=".">
     <xsl:with-param name="p" select="3"/>
   </xsl:apply-templates>
   </xsl:if>
  </xsl:for-each>
  <xsl:if test="$p &gt; 3"><mo>)</mo></xsl:if>
  </mrow>
</xsl:template>


<!-- 4.4.3.10 root -->
<xsl:template match="apply[*[1][self::root] and not(degree) or degree=2]" priority="4">
<msqrt>
<xsl:apply-templates select="*[position()&gt;1]"/>
</msqrt>
</xsl:template>

<xsl:template match="apply[*[1][self::root]]">
<mroot>
<xsl:apply-templates select="*[position()&gt;1 and not(self::degree)]"/>
<mrow><xsl:apply-templates select="degree/*"/></mrow>
</mroot>
</xsl:template>


<xsl:template match="apply[*[1][self::csymbol='root']]">
<mroot>
  <xsl:apply-templates select="*[position()!=1]"/>
</mroot>
</xsl:template>

<!-- 4.4.3.11 gcd -->
<xsl:template match="gcd">
<mi>gcd</mi>
</xsl:template>

<!-- 4.4.3.12 and -->
<xsl:template match="apply[*[1][self::and]]
                                |reln[*[1][self::and]]
                                |apply[*[1][self::csymbol='and']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>and</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>


<!-- 4.4.3.13 or -->
<xsl:template match="apply[*[1][self::or]]
                       |apply[*[1][self::csymbol='or']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="3"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>or</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.3.14 xor -->
<xsl:template match="apply[*[1][self::xor]]
                       |apply[*[1][self::csymbol='xor']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="3"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>xor</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>


<!-- 4.4.3.15 not -->
<xsl:template match="apply[*[1][self::not]]
                       |apply[*[1][self::csymbol='not']]">
<mrow>
<mo>not</mo>
<xsl:apply-templates select="*[2]">
  <xsl:with-param name="p" select="7"/>
</xsl:apply-templates>
</mrow>
</xsl:template>




<!-- 4.4.3.16 implies -->
<xsl:template match="apply[*[1][self::implies]]
                                |reln[*[1][self::implies]]
                                |apply[*[1][self::csymbol='implies']]">
  <xsl:param name="p" select="0"/>
<xsl:call-template name="binary">
  <xsl:with-param name="mo"><mo>&#8658;<!-- Rightarrow --></mo></xsl:with-param>
  <xsl:with-param name="p" select="$p"/>
  <xsl:with-param name="this-p" select="3"/>
</xsl:call-template>
</xsl:template>


<!-- 4.4.3.17 forall -->
<xsl:template match="apply[*[1][self::forall]]
                       |apply[*[1][self::csymbol='forall']]
                       |bind[*[1][self::forall]]
                       |bind[*[1][self::csymbol='forall']]">
 <mrow>
  <mo>&#8704;<!--forall--></mo>
 <mrow><xsl:apply-templates select="bvar[not(current()/condition)]/*|condition/*"/></mrow>
 <mo>.</mo>
 <mfenced>
  <xsl:apply-templates select="*[last()]"/>
 </mfenced>
</mrow>
</xsl:template>



<!-- 4.4.3.18 exists -->
<xsl:template match="apply[*[1][self::exists]]
                       |apply[*[1][self::csymbol='exists']]
                       |bind[*[1][self::exists]]
                       |bind[*[1][self::csymbol='exists']]">
 <mrow>
  <mo>&#8707;<!--exists--></mo>
 <mrow><xsl:apply-templates select="bvar[not(current()/condition)]/*|condition/*"/></mrow>
 <mo>.</mo>
 <mfenced separators="">
   <xsl:choose>
     <xsl:when test="condition">
       <xsl:apply-templates select="condition/*"/>
       <mo>&#8743;<!-- and --></mo>
     </xsl:when>
     <xsl:when test="domainofapplication">
       <mrow>
       <mrow>
         <xsl:for-each select="bvar">
           <xsl:apply-templates/>
           <xsl:if test="position()!=last()">
             <mo>,</mo>
           </xsl:if>
         </xsl:for-each>
       </mrow>
       <mo>&#8712;<!-- in --></mo>
       <xsl:apply-templates select="domainofapplication/*"/>
       </mrow>
       <mo>&#8743;<!-- and --></mo>
     </xsl:when>
   </xsl:choose>
  <xsl:apply-templates select="*[last()]"/>
 </mfenced>
</mrow>
</xsl:template>



<!-- 4.4.3.19 abs -->
<xsl:template match="apply[*[1][self::abs]]
                       |apply[*[1][self::csymbol='abs']]">
<mrow>
<mo>|</mo>
<xsl:apply-templates select="*[2]"/>
<mo>|</mo>
</mrow>
</xsl:template>



<!-- 4.4.3.20 conjugate -->
<xsl:template match="apply[*[1][self::conjugate]]
                       |apply[*[1][self::csymbol='conjugate']]">
<mover>
<xsl:apply-templates select="*[2]"/>
<mo>&#175;<!-- overline --></mo>
</mover>
</xsl:template>

<!-- 4.4.3.21 arg -->
<xsl:template match="arg">
 <mi>arg</mi>
</xsl:template>


<!-- 4.4.3.22 real -->
<xsl:template match="real|csymbol[.='real']">
 <mo>&#8475;<!-- real --></mo>
</xsl:template>

<!-- 4.4.3.23 imaginary -->
<xsl:template match="imaginary|csymbol[.='imaginary']">
 <mo>&#8465;<!-- imaginary --></mo>
</xsl:template>

<!-- 4.4.3.24 lcm -->
<xsl:template match="lcm">
 <mi>lcm</mi>
</xsl:template>


<!-- 4.4.3.25 floor -->
<xsl:template match="apply[*[1][self::floor]]
                       |apply[*[1][self::csymbol='floor']]">
<mrow>
<mo>&#8970;<!-- lfloor--></mo>
<xsl:apply-templates select="*[2]"/>
<mo>&#8971;<!-- rfloor--></mo>
</mrow>
</xsl:template>


<!-- 4.4.3.25 ceiling -->
<xsl:template match="apply[*[1][self::ceiling]]
                       |apply[*[1][self::csymbol='ceiling']]">
<mrow>
<mo>&#8968;<!-- lceil--></mo>
<xsl:apply-templates select="*[2]"/>
<mo>&#8969;<!-- rceil--></mo>
</mrow>
</xsl:template>

<!-- 4.4.4.1 eq -->
<xsl:template match="apply[*[1][self::eq]]
                                |reln[*[1][self::eq]]
                                |apply[*[1][self::csymbol='eq']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="1"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>=</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.4.2 neq -->
<xsl:template match="apply[*[1][self::neq]]
                       |apply[*[1][self::csymbol='neq']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="1"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8800;<!-- neq --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.4.3 gt -->
<xsl:template match="apply[*[1][self::gt]]
                                |reln[*[1][self::gt]]
                                |apply[*[1][self::csymbol='gt']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="1"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&gt;</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.4.4 lt -->
<xsl:template match="apply[*[1][self::lt]]
                                |reln[*[1][self::lt]]
                                |apply[*[1][self::csymbol='lt']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="1"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&lt;</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.4.5 geq -->
<xsl:template match="apply[*[1][self::geq]]
                                |apply[*[1][self::csymbol='geq']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="1"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8805;</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.4.6 leq -->
<xsl:template match="apply[*[1][self::leq]]
                       |apply[*[1][self::csymbol='leq']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="1"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8804;</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.4.7 equivalent -->
<xsl:template match="apply[*[1][self::equivalent]]
                       |apply[*[1][self::csymbol='equivalent']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="1"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8801;</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.4.8 approx -->
<xsl:template match="apply[*[1][self::approx]]
                       |apply[*[1][self::csymbol='approx']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="1"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8771;</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>


<!-- 4.4.4.9 factorof -->
<xsl:template match="apply[*[1][self::factorof]]
                       |apply[*[1][self::csymbol='factorof']]">
  <xsl:param name="p" select="0"/>
<xsl:call-template name="binary">
  <xsl:with-param name="mo"><mo>|</mo></xsl:with-param>
  <xsl:with-param name="p" select="$p"/>
  <xsl:with-param name="this-p" select="3"/>
</xsl:call-template>
</xsl:template>

<!-- 4.4.5.1 int -->
<xsl:template match="apply[*[1][self::int]]
                       |apply[*[1][self::csymbol='int']]
                       |bind[*[1][self::int]]
                       |bind[*[1][self::csymbol='int']]">
 <mrow>
 <msubsup>
  <mi>&#8747;<!--int--></mi>
 <mrow><xsl:apply-templates select="lowlimit/*|interval/*[1]|condition/*|domainofapplication/*"/></mrow>
 <mrow><xsl:apply-templates select="uplimit/*|interval/*[2]"/></mrow>
 </msubsup>
 <xsl:apply-templates select="*[last()]"/>
 <xsl:if test="bvar">
   <mi>d</mi><xsl:apply-templates select="bvar"/>
 </xsl:if>
</mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='defint']]">
<mrow>
<munder><mi>&#8747;<!--int--></mi>
<xsl:apply-templates select="*[2]"/>
</munder>
 <xsl:apply-templates select="*[last()]"/>
</mrow>
</xsl:template>

<!-- 4.4.5.2 diff -->
<xsl:template match="apply[*[1][self::diff] and not(bvar)]|
                                apply[*[1][self::csymbol='diff']]" priority="2">
 <msup>
 <mrow><xsl:apply-templates select="*[2]"/></mrow>
 <mo>&#8242;<!--prime--></mo>
 </msup>
</xsl:template>

<xsl:template match="apply[*[1][self::diff]]" priority="1">
 <mfrac>
 <xsl:choose>
 <xsl:when test="bvar/degree">
 <mrow><msup><mi mathvariant="normal">d</mi><xsl:apply-templates select="bvar/degree/node()"/></msup>
     <xsl:apply-templates select="*[last()]"/></mrow>
 <mrow><mi mathvariant="normal">d</mi><msup><xsl:apply-templates
 select="bvar/node()"/><xsl:apply-templates
 select="bvar/degree/node()"/></msup>
</mrow>
</xsl:when>
<xsl:otherwise>
 <mrow><mi mathvariant="normal">d</mi><xsl:apply-templates select="*[last()]"/></mrow>
 <mrow><mi mathvariant="normal">d</mi><xsl:apply-templates select="bvar"/></mrow>
</xsl:otherwise>
 </xsl:choose>
 </mfrac>
</xsl:template>


<!-- 4.4.5.3 partialdiff -->
<xsl:template match="apply[*[1][self::partialdiff] and list and not(bvar) and count(*)=3]" priority="2">
<mrow>
 <msub><mi>D</mi><mrow>
<xsl:for-each select="list[1]/*">
<xsl:apply-templates select="."/>
<xsl:if test="position()&lt;last()"><mo>,</mo></xsl:if>
</xsl:for-each>
</mrow></msub>
 <mrow><xsl:apply-templates select="*[3]"/></mrow>
</mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::partialdiff] and list and lambda]" priority="3">
  <mfrac>
    <mrow>
      <xsl:choose>
        <xsl:when test="count(list/*)=1">
          <mo>&#8706;<!-- partial --></mo>
        </xsl:when>
        <xsl:otherwise>
          <msup><mo>&#8706;<!-- partial --></mo>
          <mrow>
            <xsl:choose>
              <xsl:when test="degree">
                <xsl:apply-templates select="degree/node()"/>
              </xsl:when>
              <xsl:otherwise>
                <mn><xsl:value-of select="count(list/*)"/></mn>
              </xsl:otherwise>
            </xsl:choose>
          </mrow>
          </msup>
        </xsl:otherwise>
      </xsl:choose>
    <xsl:apply-templates select="lambda/*[last()]"/></mrow>
    <mrow>
     <xsl:call-template name="pddx"/>
    </mrow>
  </mfrac>
</xsl:template>

<xsl:template name="pddx">
  <xsl:param name="n" select="1"/>
  <xsl:param name="b" select="lambda/bvar"/>
  <xsl:param name="l" select="list/*"/>
  <xsl:choose>
    <xsl:when
       test="number($l[1])=number($l[2])">
     <xsl:call-template name="pddx">
     <xsl:with-param name="n" select="$n+1"/>
     <xsl:with-param name="b" select="$b"/>
     <xsl:with-param name="l" select="$l[position()!=1]"/>
     </xsl:call-template>
    </xsl:when>
   <xsl:otherwise>
     <mrow>
     <mo>&#8706;<!-- partial --></mo>
     <xsl:choose>
       <xsl:when test="$n=1">
         <xsl:apply-templates select="$b[position()=$l[1]]/*"/>
       </xsl:when>
       <xsl:otherwise>
         <msup>
           <xsl:apply-templates select="$b[position()=$l[1]]/*"/>
           <mn><xsl:value-of select="$n"/></mn>
         </msup>
       </xsl:otherwise>
     </xsl:choose>
     </mrow>
     <xsl:if test="$l[2]">
      <xsl:call-template name="pddx">
       <xsl:with-param name="b" select="$b"/>
       <xsl:with-param name="l" select="$l[position()!=1]"/>
      </xsl:call-template>
     </xsl:if>
   </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="apply[*[1][self::partialdiff]]" priority="1">
  <mfrac>
    <mrow>
      <xsl:choose>
        <xsl:when test="not(bvar/degree) and not(bvar[2])">
          <mo>&#8706;<!-- partial --></mo>
        </xsl:when>
        <xsl:otherwise>
          <msup><mo>&#8706;<!-- partial --></mo>
          <mrow>
            <xsl:choose>
              <xsl:when test="degree">
                <xsl:apply-templates select="degree/node()"/>
              </xsl:when>
              <xsl:when test="bvar/degree[string(number(.))='NaN']">
                <xsl:for-each select="bvar/degree">
                  <xsl:apply-templates select="node()"/>
                  <xsl:if test="position()&lt;last()"><mo>+</mo></xsl:if>
                </xsl:for-each>
                <xsl:if test="count(bvar[not(degree)])&gt;0">
                  <mo>+</mo><mn><xsl:value-of select="count(bvar[not(degree)])"/></mn>
                </xsl:if>
              </xsl:when>
              <xsl:otherwise>
                <mn><xsl:value-of select="number(sum(bvar/degree))+count(bvar[not(degree)])"/></mn>
              </xsl:otherwise>
            </xsl:choose>
          </mrow>
          </msup>
        </xsl:otherwise>
      </xsl:choose>
    <xsl:apply-templates select="*[last()]"/></mrow>
    <mrow>
      <xsl:for-each select="bvar">
        <mrow>
          <mo>&#8706;<!-- partial --></mo><msup><xsl:apply-templates select="node()"/>
          <mrow><xsl:apply-templates select="degree/node()"/></mrow>
        </msup>
        </mrow>
      </xsl:for-each>
    </mrow>
  </mfrac>
</xsl:template>


<xsl:template match="apply[*[1][self::csymbol='partialdiffdegree']]">
  <mrow>
   <msub>
    <mo>&#8706;<!-- partial --></mo>
    <mrow>
     <xsl:apply-templates select="*[2]"/>
    </mrow>
   </msub>
   <mfenced>
     <xsl:apply-templates select="*[4]"/>
   </mfenced>
  </mrow>
</xsl:template>


<!-- 4.4.5.4 lowlimit -->
<xsl:template match="lowlimit"/>

<!-- 4.4.5.5 uplimit -->
<xsl:template match="uplimit"/>

<!-- 4.4.5.6 bvar -->
<xsl:template match="bvar">
 <xsl:apply-templates/>
 <xsl:if test="following-sibling::bvar"><mo>,</mo></xsl:if>
</xsl:template>

<!-- 4.4.5.7 degree -->
<xsl:template match="degree"/>

<!-- 4.4.5.8 divergence -->
<xsl:template match="divergence">
<mi>div</mi>
</xsl:template>

<xsl:template match="apply[*[1][self::divergence]and bvar and vector]">
<xsl:variable name="v" select="bvar"/>
<mrow>
<mi>div</mi>
<mo>&#8289;<!--function application--></mo>
<mo>(</mo>
<mtable>
<xsl:for-each select="vector/*">
<xsl:variable name="p" select="position()"/>
<mtr><mtd>
<xsl:apply-templates select="$v[$p]/*"/>
<mo>&#x21a6;<!-- map--></mo>
<xsl:apply-templates select="."/>
</mtd></mtr>
</xsl:for-each>
</mtable>
<mo>)</mo>
</mrow>
</xsl:template>

<!-- 4.4.5.9 grad -->
<xsl:template match="grad">
<mi>grad</mi>
</xsl:template>

<xsl:template match="apply[*[1][self::grad]and bvar]">
<mrow>
<mi>grad</mi>
<mo>&#8289;<!--function application--></mo>
<mrow>
<mo>(</mo>
<mfenced>
<xsl:apply-templates select="bvar/*"/>
</mfenced>
<mo>&#x21a6;<!-- map--></mo>
<xsl:apply-templates select="*[position()!=1][not(self::bvar)]"/>
<mo>)</mo>
</mrow>
</mrow>
</xsl:template>

<!-- 4.4.5.10 curl -->
<xsl:template match="curl">
<mi>curl</mi>
</xsl:template>


<!-- 4.4.5.11 laplacian -->
<xsl:template match="laplacian">
<msup><mo>&#8711;<!-- nabla --></mo><mn>2</mn></msup>
</xsl:template>

<xsl:template match="apply[*[1][self::laplacian]and bvar]">
<mrow>
<xsl:apply-templates select="*[1]"/>
<mo>&#8289;<!--function application--></mo>
<mrow>
<mo>(</mo>
<mfenced>
<xsl:apply-templates select="bvar/*"/>
</mfenced>
<mo>&#x21a6;<!-- map--></mo>
<xsl:apply-templates select="*[position()!=1][not(self::bvar)]"/>
<mo>)</mo>
</mrow>
</mrow>
</xsl:template>

<!-- 4.4.6.1 set -->

<xsl:template match="set">
  <xsl:call-template name="set"/>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='set']]">
<mfenced open="{{" close="}}" separators=",">
  <xsl:apply-templates select="*[position()!=1]"/>
</mfenced>
</xsl:template>

<!-- 4.4.6.2 list -->

<xsl:template match="list">
  <xsl:call-template name="set">
   <xsl:with-param name="o" select="'('"/>
   <xsl:with-param name="c" select="')'"/>
  </xsl:call-template>
</xsl:template>


<xsl:template match="apply[*[1][self::csymbol='list']]">
<mfenced open="(" close=")" separators=",">
  <xsl:apply-templates select="*[position()!=1]"/>
</mfenced>
</xsl:template>

<!-- 4.4.6.3 union -->
<xsl:template match="apply[*[1][self::union]]
                       |apply[*[1][self::csymbol='union']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8746;<!-- union --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<xsl:template match="apply[*[1][self::union]][bvar]
                                |apply[*[1][self::csymbol='union']][bvar]"
              priority="2"
>
  <xsl:call-template name="sum">
    <xsl:with-param name="mo"><mo>&#x22C3;</mo></xsl:with-param>
  </xsl:call-template>
</xsl:template>

<!-- 4.4.6.4 intersect -->
<xsl:template match="apply[*[1][self::intersect]]
                       |apply[*[1][self::csymbol='intersect']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="3"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8745;<!-- intersect --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>


<xsl:template match="apply[*[1][self::intersect]][bvar]
                                |apply[*[1][self::csymbol='intersect']][bvar]"
              priority="2"
>
  <xsl:call-template name="sum">
    <xsl:with-param name="mo"><mo>&#x22C2;</mo></xsl:with-param>
  </xsl:call-template>
</xsl:template>



<!-- 4.4.6.5 in -->
<xsl:template match="apply[*[1][self::in]]
                       |apply[*[1][self::csymbol='in']]">
  <xsl:param name="p" select="0"/>
<xsl:call-template name="binary">
  <xsl:with-param name="mo"><mo>&#8712;<!-- in --></mo></xsl:with-param>
  <xsl:with-param name="p" select="$p"/>
  <xsl:with-param name="this-p" select="3"/>
</xsl:call-template>
</xsl:template>

<!-- 4.4.6.5 notin -->
<xsl:template match="apply[*[1][self::notin]]|reln[*[1][self::notin]]
                       |apply[*[1][self::csymbol='notin']]">
  <xsl:param name="p" select="0"/>
<xsl:call-template name="binary">
  <xsl:with-param name="mo"><mo>&#8713;<!-- not in --></mo></xsl:with-param>
  <xsl:with-param name="p" select="$p"/>
  <xsl:with-param name="this-p" select="3"/>
</xsl:call-template>
</xsl:template>

<!-- 4.4.6.7 subset -->
<xsl:template match="apply[*[1][self::subset]]
                       |apply[*[1][self::csymbol='subset']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8838;<!-- subseteq --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.6.8 prsubset -->
<xsl:template match="apply[*[1][self::prsubset]]
                       |apply[*[1][self::csymbol='prsubset']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8834;<!-- prsubset --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.6.9 notsubset -->
<xsl:template match="apply[*[1][self::notsubset]]
                       |apply[*[1][self::csymbol='notsubset']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="binary">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8840;<!-- notsubseteq --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.6.10 notprsubset -->
<xsl:template match="apply[*[1][self::notprsubset]]
                       |apply[*[1][self::csymbol='notprsubset']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="binary">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8836;<!-- prsubset --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.6.11 setdiff -->
<xsl:template match="apply[*[1][self::setdiff]]
                       |apply[*[1][self::csymbol='setdiff']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="binary">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#8726;<!-- setminus --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.6.12 card -->
<xsl:template match="apply[*[1][self::card]]
                       |apply[*[1][self::csymbol='card']]">
<mrow>
<mo>|</mo>
<xsl:apply-templates select="*[2]"/>
<mo>|</mo>
</mrow>
</xsl:template>

<!-- 4.4.6.13 cartesianproduct -->
<xsl:template match="apply[*[1][self::cartesianproduct or self::vectorproduct]]
                                |apply[*[1][self::csymbol[.='cartesian_product' or . = 'vectorproduct']]]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#215;<!-- times --></mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<xsl:template
match="apply[*[1][self::cartesianproduct][count(following-sibling::reals)=count(following-sibling::*)]]"
priority="2">
<msup>
<xsl:apply-templates select="*[2]">
  <xsl:with-param name="p" select="5"/>
</xsl:apply-templates>
<mn><xsl:value-of select="count(*)-1"/></mn>
</msup>
</xsl:template>


<!-- 4.4.7.1 sum -->
<xsl:template name="sum" match="apply[*[1][self::sum]]">
  <xsl:param name="mo"><mo>&#8721;<!--sum--></mo></xsl:param>
 <mrow>
 <munderover>
  <xsl:copy-of select="$mo"/>
 <mrow><xsl:apply-templates select="lowlimit|interval/*[1]|condition/*|domainofapplication/*"/></mrow><!-- Alexey Shamrin shamrinATmail.ru -->
 <mrow><xsl:apply-templates select="uplimit/*|interval/*[2]"/></mrow>
 </munderover>
 <xsl:apply-templates select="*[last()]"/>
</mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='sum']]">
<mrow>
<munder><mo>&#8721;<!--sum--></mo>
<xsl:apply-templates select="*[2]"/>
</munder>
 <xsl:apply-templates select="*[last()]"/>
</mrow>
</xsl:template>

<xsl:template match="apply/lowlimit" priority="3">
<mrow>
<xsl:if test="../bvar">
  <xsl:apply-templates select="../bvar/node()"/>
  <mo>=</mo>
</xsl:if>
<xsl:apply-templates/>
</mrow>
</xsl:template>

<!-- 4.4.7.2 product -->
<xsl:template match="apply[*[1][self::product]]">
  <xsl:call-template name="sum">
    <xsl:with-param name="mo"><mo>&#8719;<!--product--></mo></xsl:with-param>
  </xsl:call-template>
</xsl:template>


<xsl:template match="apply[*[1][self::csymbol='product']]">
<mrow>
<munder><mo>&#8719;<!--product--></mo>
<xsl:apply-templates select="*[2]"/>
</munder>
 <xsl:apply-templates select="*[last()]"/>
</mrow>
</xsl:template>

<!-- 4.4.7.3 limit -->
<xsl:template match="apply[*[1][self::limit]]">
 <mrow>
 <munder>
  <mi>lim</mi> <!-- Alexey Shamrin shamrinATmail.ru -->
 <mrow><xsl:apply-templates select="lowlimit|condition/*"/></mrow>
 </munder>
 <xsl:apply-templates select="*[last()]"/>
</mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='limit']][bind]">
 <mrow>
 <munder>
  <mi>lim</mi>
 <mrow>
 <xsl:apply-templates select="bind/bvar/*"/>
    <mo>
      <xsl:choose>
        <xsl:when test="*[3]='above'">&#8600;<!--searrow--></xsl:when>
        <xsl:when test="*[3]='below'">&#8599;<!--nearrow--></xsl:when>
        <xsl:otherwise>&#8594;<!--rightarrow--></xsl:otherwise>
      </xsl:choose>
    </mo>
 <xsl:apply-templates select="*[2]"/>
</mrow>
 </munder>
 <xsl:apply-templates select="bind/*[last()]"/>
</mrow>
</xsl:template>



<xsl:template match="apply[limit]/lowlimit" priority="4">
<mrow>
<xsl:apply-templates select="../bvar/node()"/>
<mo>&#8594;<!--rightarrow--></mo>
<xsl:apply-templates/>
</mrow>
</xsl:template>


<!-- 4.4.7.4 tendsto -->
<xsl:template match="apply[*[1][self::tendsto]]|reln[*[1][self::tendsto]]">
 <xsl:param name="p"/>
<xsl:call-template name="binary">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>
  <xsl:choose>
   <xsl:when test="@type='above'">&#8600;<!--searrow--></xsl:when>
   <xsl:when test="@type='below'">&#8599;<!--nearrow--></xsl:when>
   <xsl:when test="@type='two-sided'">&#8594;<!--rightarrow--></xsl:when>
   <xsl:otherwise>&#8594;<!--rightarrow--></xsl:otherwise>
  </xsl:choose>
  </mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='tendsto']]">
  <mrow>
    <xsl:apply-templates select="*[3]"/>
    <mo>
      <xsl:choose>
        <xsl:when test="*[1][self::above]">&#8600;<!--searrow--></xsl:when>
        <xsl:when test="*[1][self::below]">&#8599;<!--nearrow--></xsl:when>
        <xsl:when test="*[1][self::two-sided]">&#8594;<!--rightarrow--></xsl:when>
        <xsl:otherwise>&#8594;<!--rightarrow--></xsl:otherwise>
      </xsl:choose>
    </mo>
    <xsl:apply-templates select="*[4]"/>
  </mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::semantics/ci='tendsto']]">
  <mrow>
    <xsl:apply-templates select="*[2]"/>
    <mo>&#8594;<!--rightarrow--></mo>
    <xsl:apply-templates select="*[3]"/>
  </mrow>
</xsl:template>

<xsl:template match="tendsto">
 <mi>tendsto</mi>
</xsl:template>

<!-- 4.4.8.1 trig -->
<xsl:template match="apply[*[1][
 self::sin or self::cos or self::tan or self::sec or
 self::csc or self::cot or self::sinh or self::cosh or
 self::tanh or self::sech or self::csch or self::coth or
 self::arcsin or self::arccos or self::arctan or self::arccosh
 or self::arccot or self::arccoth or self::arccsc or
 self::arccsch or self::arcsec or self::arcsech or
 self::arcsinh or self::arctanh or self::ln]]">
  <mrow>
    <mi><xsl:value-of select="local-name(*[1])"/></mi>
    <mo>&#8289;<!--function application--></mo>
    <xsl:if test="apply"><mo>(</mo></xsl:if>
    <xsl:apply-templates select="*[2]"/>
    <xsl:if test="apply"><mo>)</mo></xsl:if>
  </mrow>
</xsl:template>

<!-- Vasil I. Yaroshevich -->
<xsl:template match="
 sin | cos | tan | sec |
 csc | cot | sinh | cosh |
 tanh | sech | csch | coth |
 arcsin | arccos | arctan | arccosh
 | arccot | arccoth | arccsc |
 arccsch | arcsec | arcsech |
 arcsinh | arctanh | ln|mean|
 plus|minus">
<mi><xsl:value-of select="local-name()"/></mi>
</xsl:template>




<!-- 4.4.8.2 exp -->
<xsl:template match="apply[*[1][self::exp]]
                       |apply[*[1][self::csymbol='exp']]">
<msup>
<mi>e<!-- exponential e--></mi>
<mrow><xsl:apply-templates select="*[2]"/></mrow>
</msup>
</xsl:template>

<!-- 4.4.8.3 ln -->
<!-- with trig -->

<!-- 4.4.8.4 log -->
<xsl:template match="apply[*[1][self::log]]
                       |apply[*[1][self::csymbol='log']]">
<mrow>
<xsl:choose>
<xsl:when test="not(logbase) or logbase=10">
<mi>log</mi>
</xsl:when>
<xsl:otherwise>
<msub>
<mi>log</mi>
<mrow><xsl:apply-templates select="logbase/node()"/></mrow>
</msub>
</xsl:otherwise>
</xsl:choose>
<mo>&#8289;<!--function application--></mo>
<xsl:apply-templates select="*[last()]">
  <xsl:with-param name="p" select="7"/>
</xsl:apply-templates>
</mrow>
</xsl:template>


<!-- 4.4.9.1 mean -->
<xsl:template match="apply[*[1][self::mean]]
                       |apply[*[1][self::csymbol='mean']]">
<mrow>
 <mo>&#9001;<!--langle--></mo>
    <xsl:for-each select="*[position()&gt;1]">
      <xsl:apply-templates select="."/>
      <xsl:if test="position() !=last()"><mo>,</mo></xsl:if>
    </xsl:for-each>
<mo>&#9002;<!--rangle--></mo>
</mrow>
</xsl:template>


<!-- 4.4.9.2 sdef -->
<xsl:template match="sdev|csymbol[.='sdev']">
<mo>&#963;<!--sigma--></mo>
</xsl:template>

<!-- 4.4.9.3 variance -->
<xsl:template match="apply[*[1][self::variance]]
                       |apply[*[1][self::csymbol='variance']]">
<msup>
<mrow>
<mo>&#963;<!--sigma--></mo>
 <mo>&#8289;<!--function application--></mo>
<mfenced>
<xsl:apply-templates select="*[position()!=1]"/>
</mfenced>
</mrow>
<mn>2</mn>
</msup>
</xsl:template>


<!-- 4.4.9.4 median -->
<xsl:template match="median">
<mi>median</mi>
</xsl:template>


<!-- 4.4.9.5 mode -->
<xsl:template match="mode">
<mi>mode</mi>
</xsl:template>

<!-- 4.4.9.5 moment -->
<xsl:template match="apply[*[1][self::moment]]">
  <mrow>
    <mo>&#9001;<!--langle--></mo>
    <msup>
      <xsl:variable name="data"
                    select="*[not(position()=1)]
                            [not(self::degree or self::momentabout)]"/>
      <xsl:choose>
        <xsl:when test="$data[2]">
          <mfenced>
            <xsl:apply-templates select="$data"/>
          </mfenced>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates select="$data"/>
        </xsl:otherwise>
      </xsl:choose>
      <mrow><xsl:apply-templates select="degree/node()"/></mrow>
    </msup>
    <mo>&#9002;<!--rangle--></mo>
  </mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='moment']]">
<msub>
  <mrow>
    <mo>&#9001;<!--langle--></mo>
    <msup>
          <xsl:apply-templates select="*[4]"/>
          <xsl:apply-templates select="*[2]"/>
    </msup>
    <mo>&#9002;<!--rangle--></mo>
  </mrow>
  <xsl:apply-templates select="*[3]"/>
</msub>
</xsl:template>

<!-- 4.4.9.5 momentabout -->
<xsl:template match="momentabout"/>

<xsl:template match="apply[*[1][self::moment]][momentabout]" priority="2">
  <msub>
    <mrow>
      <mo>&#9001;<!--langle--></mo>
      <msup>
        <xsl:variable name="data"
                      select="*[not(position()=1)]
                              [not(self::degree or self::momentabout)]"/>
        <xsl:choose>
          <xsl:when test="$data[2]">
            <mfenced>
              <xsl:apply-templates select="$data"/>
            </mfenced>
          </xsl:when>
          <xsl:otherwise>
            <xsl:apply-templates select="$data"/>
          </xsl:otherwise>
        </xsl:choose>
        <mrow><xsl:apply-templates select="degree/node()"/></mrow>
      </msup>
      <mo>&#9002;<!--rangle--></mo>
    </mrow>
    <mrow>
      <xsl:apply-templates select="momentabout/*"/>
    </mrow>
  </msub>
</xsl:template>

<!-- 4.4.10.1 vector -->
<xsl:template match="vector">
<mrow>
<mo>(</mo>
<mtable>
<xsl:for-each select="*">
<mtr><mtd><xsl:apply-templates select="."/></mtd></mtr>
</xsl:for-each>
</mtable>
<mo>)</mo>
</mrow>
</xsl:template>


<xsl:template match="vector[condition]">
  <mrow>
    <mo>[</mo>
    <xsl:apply-templates select="*[last()]"/>
    <mo>|</mo>
    <xsl:apply-templates select="condition"/>
    <mo>]</mo>
  </mrow>
</xsl:template>

<xsl:template match="vector[domainofapplication]">
  <mrow>
    <mo>[</mo>
    <xsl:apply-templates select="*[last()]"/>
    <mo>|</mo>
    <xsl:apply-templates select="bvar/*"/>
    <mo>&#x2208;</mo>
    <xsl:apply-templates select="domainofapplication/*"/>
    <mo>]</mo>
  </mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='vector']]">
<mrow>
<mo>(</mo>
<mtable>
<xsl:for-each select="*[position()!=1]">
<mtr>
  <mtd><xsl:apply-templates select="."/></mtd>
</mtr>
</xsl:for-each>
</mtable>
<mo>)</mo>
</mrow>
</xsl:template>

<!-- 4.4.10.2 matrix -->
<xsl:template match="matrix">
<mrow>
<mo>(</mo>
<mtable>
<xsl:apply-templates/>
</mtable>
<mo>)</mo>
</mrow>
</xsl:template>

<xsl:template match="matrix[condition]">
  <mrow>
    <mo>[</mo>
    <msub>
      <mi>m</mi>
      <mrow>
        <xsl:for-each select="bvar">
          <xsl:apply-templates/>
          <xsl:if test="position()!=last()"><mo>,</mo></xsl:if>
        </xsl:for-each>
      </mrow>
    </msub>
    <mo>|</mo>
    <mrow>
      <msub>
        <mi>m</mi>
        <mrow>
          <xsl:for-each select="bvar">
            <xsl:apply-templates/>
            <xsl:if test="position()!=last()"><mo>,</mo></xsl:if>
          </xsl:for-each>
        </mrow>
      </msub>
      <mo>=</mo>
      <xsl:apply-templates select="*[last()]"/>
    </mrow>
    <mo>;</mo>
    <xsl:apply-templates select="condition"/>
    <mo>]</mo>
  </mrow>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol='matrix']]">
<mrow>
<mo>(</mo>
<mtable>
<xsl:apply-templates select="*[position()!=1]"/>
</mtable>
<mo>)</mo>
</mrow>
</xsl:template>


<!-- 4.4.10.3 matrixrow -->
<xsl:template match="matrix/matrixrow">
<mtr>
<xsl:for-each select="*">
<mtd><xsl:apply-templates select="."/></mtd>
</xsl:for-each>
</mtr>
</xsl:template>

<xsl:template match="matrixrow">
<mtable>
<mtr>
<xsl:for-each select="*">
<mtd><xsl:apply-templates select="."/></mtd>
</xsl:for-each>
</mtr>
</mtable>
</xsl:template>

<xsl:template match="apply[*[1][self::csymbol.='matrixrow']]">
<mtr>
<xsl:for-each select="*[position()!=1]">
<mtd><xsl:apply-templates select="."/></mtd>
</xsl:for-each>
</mtr>
</xsl:template>

<!-- 4.4.10.4 determinant -->
<xsl:template match="apply[*[1][self::determinant]]
                       |apply[*[1][self::csymbol='determinant']]">
<mrow>
<mi>det</mi>
 <mo>&#8289;<!--function application--></mo>
<xsl:apply-templates select="*[2]">
  <xsl:with-param name="p" select="7"/>
</xsl:apply-templates>
</mrow>
</xsl:template>

<xsl:template
match="apply[*[1][self::determinant]][*[2][self::matrix]]" priority="2">
<mrow>
<mo>|</mo>
<mtable>
<xsl:apply-templates select="matrix/*"/>
</mtable>
<mo>|</mo>
</mrow>
</xsl:template>

<!-- 4.4.10.5 transpose -->
<xsl:template match="apply[*[1][self::transpose]]
                       |apply[*[1][self::csymbol='transpose']]">
<msup>
<xsl:apply-templates select="*[2]">
  <xsl:with-param name="p" select="7"/>
</xsl:apply-templates>
<mi>T</mi>
</msup>
</xsl:template>

<!-- 4.4.10.5 selector -->
<xsl:template match="apply[*[1][self::selector]]
                       |apply[*[1][self::csymbol='selector']]">
<msub>
<xsl:apply-templates select="*[2]">
  <xsl:with-param name="p" select="7"/>
</xsl:apply-templates>
<mrow>
    <xsl:for-each select="*[position()&gt;2]">
      <xsl:apply-templates select="."/>
      <xsl:if test="position() !=last()"><mo>,</mo></xsl:if>
    </xsl:for-each>
</mrow>
</msub>
</xsl:template>

<!-- *** -->
<!-- 4.4.10.6 vectorproduct see cartesianproduct -->


<!-- 4.4.10.7 scalarproduct -->
<xsl:template match="apply[*[1][self::scalarproduct]]
                       |apply[*[1][self::csymbol='scalarproduct']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>.</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.10.8 outerproduct -->

<xsl:template match="apply[*[1][self::outerproduct]]
                       |apply[*[1][self::csymbol='outerproduct']]">
<xsl:param name="p" select="0"/>
<xsl:call-template name="infix">
 <xsl:with-param name="this-p" select="2"/>
 <xsl:with-param name="p" select="$p"/>
 <xsl:with-param name="mo"><mo>&#x2297;</mo></xsl:with-param>
</xsl:call-template>
</xsl:template>

<!-- 4.4.11.2 semantics -->
<xsl:template match="semantics">
 <xsl:apply-templates select="*[1]"/>
</xsl:template>
<xsl:template match="semantics[annotation-xml/@encoding='MathML-Presentation']">
 <xsl:apply-templates select="annotation-xml[@encoding='MathML-Presentation']/node()"/>
</xsl:template>

<!-- 4.4.12.1 integers -->
<xsl:template match="integers">
<mi mathvariant="double-struck">Z</mi>
</xsl:template>

<!-- 4.4.12.2 reals -->
<xsl:template match="reals">
<mi mathvariant="double-struck">R</mi>
</xsl:template>

<!-- 4.4.12.3 rationals -->
<xsl:template match="rationals">
<mi mathvariant="double-struck">Q</mi>
</xsl:template>

<!-- 4.4.12.4 naturalnumbers -->
<xsl:template match="naturalnumbers">
<mi mathvariant="double-struck">N</mi>
</xsl:template>

<!-- 4.4.12.5 complexes -->
<xsl:template match="complexes">
<mi mathvariant="double-struck">C</mi>
</xsl:template>

<!-- 4.4.12.6 primes -->
<xsl:template match="primes">
<mi mathvariant="double-struck">P</mi>
</xsl:template>

<!-- 4.4.12.7 exponentiale -->
<xsl:template match="exponentiale">
  <mi>e<!-- exponential e--></mi>
</xsl:template>

<!-- 4.4.12.8 imaginaryi -->
<xsl:template match="imaginaryi">
  <mi>i<!-- imaginary i--></mi>
</xsl:template>

<!-- 4.4.12.9 notanumber -->
<xsl:template match="notanumber">
  <mi>NaN</mi>
</xsl:template>

<!-- 4.4.12.10 true -->
<xsl:template match="true">
  <mi>true</mi>
</xsl:template>

<!-- 4.4.12.11 false -->
<xsl:template match="false">
  <mi>false</mi>
</xsl:template>

<!-- 4.4.12.12 emptyset -->
<xsl:template match="emptyset|csymbol[.='emptyset']">
  <mi>&#8709;<!-- emptyset --></mi>
</xsl:template>


<!-- 4.4.12.13 pi -->
<xsl:template match="pi|csymbol[.='pi']">
  <mi>&#960;<!-- pi --></mi>
</xsl:template>

<!-- 4.4.12.14 eulergamma -->
<xsl:template match="eulergamma|csymbol[.='gamma']">
  <mi>&#947;<!-- gamma --></mi>
</xsl:template>

<!-- 4.4.12.15 infinity -->
<xsl:template match="infinity|csymbol[.='infinity']">
  <mi>&#8734;<!-- infinity --></mi>
</xsl:template>


<!-- ****************************** -->
<xsl:template name="infix" >
  <xsl:param name="mo"/>
  <xsl:param name="p" select="0"/>
  <xsl:param name="this-p" select="0"/>
  <xsl:variable name="dmo">
    <xsl:choose>
     <xsl:when test="domainofapplication">
      <munder>
       <xsl:copy-of select="$mo"/>
       <mrow>
        <xsl:apply-templates select="domainofapplication/*"/>
       </mrow>
      </munder>
     </xsl:when>
     <xsl:otherwise>
       <xsl:copy-of select="$mo"/>
     </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <mrow>
  <xsl:if test="$this-p &lt; $p"><mo>(</mo></xsl:if>
  <xsl:for-each select="*[not(self::domainofapplication)][position()&gt;1]">
   <xsl:if test="position() &gt; 1">
    <xsl:copy-of select="$dmo"/>
   </xsl:if>
   <xsl:apply-templates select=".">
     <xsl:with-param name="p" select="$this-p"/>
   </xsl:apply-templates>
  </xsl:for-each>
  <xsl:if test="$this-p &lt; $p"><mo>)</mo></xsl:if>
  </mrow>
</xsl:template>

 <!-- make 1-(9-2) google code isse 3 -->
<xsl:template name="binary" >
  <xsl:param name="mo"/>
  <xsl:param name="p" select="0"/>
  <xsl:param name="this-p" select="0"/>
  <mrow>
  <xsl:if test="($this-p &lt; $p) or ($this-p=$p and $mo='&#8722;')"><mo>(</mo></xsl:if>
   <xsl:apply-templates select="*[2]">
     <xsl:with-param name="p" select="$this-p"/>
   </xsl:apply-templates>
   <xsl:copy-of select="$mo"/>
   <xsl:apply-templates select="*[3]">
     <xsl:with-param name="p" select="$this-p"/>
   </xsl:apply-templates>
  <xsl:if test="($this-p &lt; $p) or ($this-p=$p and $mo='&#8722;')"><mo>)</mo></xsl:if>
  </mrow>
</xsl:template>

<xsl:template name="set" >
  <xsl:param name="o" select="'{'"/>
  <xsl:param name="c" select="'}'"/>
  <mrow>
   <mo><xsl:value-of select="$o"/></mo>
   <xsl:choose>
   <xsl:when test="condition">
   <mrow><xsl:apply-templates select="condition/following-sibling::*"/></mrow>
   <mo>|</mo>
   <mrow><xsl:apply-templates select="condition/node()"/></mrow>
   </xsl:when>
   <xsl:when test="domainofapplication">
    <mrow><xsl:apply-templates select="domainofapplication/following-sibling::*"/></mrow>
    <mo>|</mo>
    <mrow><xsl:apply-templates select="bvar/node()"/></mrow>
    <mo>&#8712;<!-- in --></mo>
    <mrow><xsl:apply-templates select="domainofapplication/node()"/></mrow>
   </xsl:when>
   <xsl:otherwise>
    <xsl:for-each select="*[not(position()=1 and parent::apply)]">
      <xsl:apply-templates select="."/>
      <xsl:if test="position() !=last()"><mo>,</mo></xsl:if>
    </xsl:for-each>
   </xsl:otherwise>
   </xsl:choose>
   <mo><xsl:value-of select="$c"/></mo>
  </mrow>
</xsl:template>


<!-- mathml 3 addtitions -->

<xsl:template match="cs">
  <ms>
   <xsl:value-of select="
                         translate(.,
                         '&#9;&#10;&#13;&#32;',
                         '&#160;&#160;&#160;&#160;')"/>
 </ms>
</xsl:template>

<xsl:template match="cbytes">
 <mrow/>
</xsl:template>

<xsl:template match="cerror">
 <merror>
   <xsl:apply-templates/>
 </merror>
</xsl:template>

<xsl:template match="share" priority="4">
 <mi href="{@href}">share<xsl:value-of select="substring-after(@href,'#')"/></mi>
</xsl:template>

</xsl:stylesheet>
