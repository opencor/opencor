The original version of ctop.xsl can be found at http://code.google.com/p/web-xslt/source/browse/trunk/ctop/ctop.xsl. However, the following Content MathML:

    <math xmlns="http://www.w3.org/1998/Math/MathML">
        <apply>
            <eq/>
            <apply>
                <diff/>
                <bvar>
                    <ci>t</ci>
                </bvar>
                <ci>V</ci>
            </apply>
            <apply>
                <divide/>
                <apply>
                    <minus/>
                    <apply>
                        <plus/>
                        <ci>i_Na</ci>
                        <ci>i_K</ci>
                        <ci>i_Leak</ci>
                    </apply>
                </apply>
                <ci>Cm</ci>
            </apply>
        </apply>
    </math>

would get translated as:

    <math xmlns="http://www.w3.org/1998/Math/MathML">
        <m:mrow xmlns:m="http://www.w3.org/1998/Math/MathML">
            <m:mfrac>
                <m:mrow>
                    <m:mi>d</m:mi>
                    <m:mrow>
                        <m:mi>V</m:mi>
                    </m:mrow>
                </m:mrow>
                <m:mrow>
                    <m:mi>d</m:mi>
                    <m:mi>
                        <m:mrow>
                            <m:mi>t</m:mi>
                        </m:mrow>
                    </m:mi>
                </m:mrow>
            </m:mfrac>
            <m:mo>=</m:mo>
            <m:mrow>
                <m:mrow>
                    <m:mo>-</m:mo>
                    <m:mrow>
                        <m:mo>(</m:mo>
                        <m:mrow>
                            <m:mi>i_Na</m:mi>
                        </m:mrow>
                        <m:mo>+</m:mo>
                        <m:mrow>
                            <m:mi>i_K</m:mi>
                        </m:mrow>
                        <m:mo>+</m:mo>
                        <m:mrow>
                            <m:mi>i_Leak</m:mi>
                        </m:mrow>
                        <m:mo>)</m:mo>
                    </m:mrow>
                </m:mrow>
                <m:mo>/</m:mo>
                <m:mrow>
                    <m:mi>Cm</m:mi>
                </m:mrow>
            </m:mrow>
        </m:mrow>
    </math>

which doesn't get rendered as expected using our Viewer plugin and this for two main reasons:
 - We have <m:xxx>...</m:xxx> while our viewer expects <xxx>...</xxx>; and
 - The bvar element gets translated as <mi><mrow><mi>...</mi></mrow></mi> while our viewer expects an mi element to have a text child.

Also, the divide element gets translated as <mrow>...</mrow><mo>/</mo><mrow>...</mrow> while, ideally, it would get translated as <mfrac><mrow>...</mrow><mrow>...</mrow></mfrac>.

All of these issues are addressed in this new version of ctop.xsl. For information, the divide element issue was addressed using the solution provided by Wikiwide on 9 June 2013 (see https://addons.mozilla.org/en-US/firefox/addon/mathml-ctop/).

Some additional improvements are:
 - Improvements to the way some mi elements are rendered (see https://github.com/opencor/opencor/issues/297); and
 - Use of middle dots rather than invisible multiplication signs (see https://github.com/opencor/opencor/issues/301).
