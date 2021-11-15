# import xml.etree.ElementTree as ET
# document = ET.parse("modelDescription.xml")
# root = document.getroot()
# for child in root.iter("ModelVariables"):
#     for c2 in child:
#         print(c2.tag,c2.attrib)
#         for c3 in c2:
#             print(c3.tag,c3.attrib)
#     # print({x.attrib for x in root.findall(child.tag+"/*")})

# node = ET.SubElement(root, 'ModelVariables')
# print(node)
# for c2 in node:
#     print(c2.tag,c2.attrib)
#     for c3 in c2:
#         print(c3.tag,c3.attrib)
# et = ET.ElementTree(document)

# f = BytesIO()
# et.write(f, encoding='utf-8', xml_declaration=True) 
# print(f.getvalue())  # your XML file, encoded as UTF-8

from xml.etree.ElementTree import ElementTree, Element, SubElement, tostring
from xml.dom.minidom import parseString
import xml.etree.ElementTree as ET
import copy

document = ET.parse("modelDescription.xml")
doc_root = document.getroot()
root = Element('ModelVariables')

# for c in file.findall(".//c"):
#     dupe = copy.deepcopy(c) #copy <c> node
#     b.append(dupe) #insert the new node
# document = ET.parse("modelDescription.xml")
# root = document.getroot()
for c1 in doc_root.iter("ModelVariables"):
    for c2 in c1.iter("ScalarVariable"):
        dupe = copy.deepcopy(c2) #copy <c> node
        root.append(dupe) #insert the new node

# for node in doc_root.findall("//ModelVariables/*"):
#     for type in node.getchildren():
#         print(type.text)
ET.dump(root)
# print(ET.dump(root))
# model_var = SubElement(doc_root, "ModelVariables")
# print(model_var)
# for c2 in model_var:
#     print(c2.tag,c2.attrib)
#     for c3 in c2:
#         print(c3.tag,c3.attrib)
# tree = ElementTree(model_var)
# print(tree)

# with open('model.xml', 'w') as f:
#     tree.write(f, encoding='unicode')
# et = ET.ElementTree(model_var)
# xml_str = tostring(tree, "UTF-8")
# print(xml_str)
# et.write(open('test.xml', 'w'))

