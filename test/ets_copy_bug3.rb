require 'xml'

str = <<-STR
  <html>
     <body>
         <div class="textarea" id="t1"
  style="STATIC">werwerwerwerwer                 </div>
         <div class="textarea" id="t2" style="STATIC">
             Quisque et diam dapibus nisi bibendum blandit.
         </div>
         <div class="textarea" id="t3" style="STATIC">
             <p>aaaaaaaaa</p>
         </div>
     </body>
  </html>
STR

XML::Parser.default_keep_blanks = false
xp = XML::Parser.new
xp.string = str
doc = xp.parse

xpath = "//div[@id='t1']"
div1 = doc.find(xpath).to_a[0]
printf "xxx div1: #{div1}\n"

xpath = "//div[@id='t2']"
div2 = doc.find(xpath).to_a[0]
printf "xxx div2: #{div2}\n"


div2.each do |child|
   #c = child.clone
   c = child.copy(false)
   div1.child_add(c)
end

printf "xxx root: #{doc.root}\n"
