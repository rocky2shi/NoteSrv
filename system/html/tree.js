/************************************************************
*                                                           *
*   作者: rocky                                             *
*   功能: 创建树形   		                                *
*                                                           *
************************************************************/

str = '';
str += '<style>';
str += '.tree_branch{cursor: pointer;	margin-top:3px;	margin-right:3px;color:red;}';
str += '.tree_leaf_open{cursor: pointer;display:block;margin-left:30px;	margin-top:3px;}';
str += '.tree_leaf_close{cursor: pointer;cursor: hand;display:none;	margin-left:30px;margin-top:3px;}';
str += '</style>';
document.write(str);

function Tree(id, bOpen)
{
	try
	{
		this.root 	= document.getElementById(id);
		this.leaf 	= document.getElementById(id + 'S');
		this.swap 	= Swap;
		this.create = Create;
		if(null==this.leaf || null==this.root)
			throw 0;
		this.isError= false;
		this.create(bOpen);
	}
	catch(e)
	{
		this.isError = true;
	}
	
	this.obj = this;
}

function Swap()
{
	thisObj = this.tree;
	if(thisObj.isError)
		return false;
	if(null==thisObj.leaf || null==thisObj.root)
		return;
	if(thisObj.leaf.className == 'tree_leaf_open')
	{
		thisObj.root.innerHTML = '＋';
		thisObj.leaf.className = 'tree_leaf_close';
	}
	else
	{
		thisObj.root.innerHTML = '－';
		thisObj.leaf.className = 'tree_leaf_open';
	}
}
function Create(bOpen)
{
	if(this.isError)
		return false;
	this.root.tree = this;
	this.root.innerHTML = bOpen?'－':'＋';
	this.root.className = 'tree_branch';
	this.root.onclick	= this.swap;
	this.leaf.className = bOpen?'tree_leaf_open':'tree_leaf_close';
}


/*************************************************
*用法示例：                                      *
*                                                *
* <span id=trees></span>第一层                   *
* <span id=treesS>                               *
* 	第二层1<br>                                  *
* 	第二层2<br>                                  *
* 	<span id=tree2></span>第二层3<br>            *
* 	<span id=tree2S>                             *
* 		第三层1<br>                              *
* 		第三层2<br>                              *
* 	</span>                                      *
* </span>                                        *
*                                                *
*                                                *
* <script>                                       *
* new Tree('trees', 1);                          *
* new Tree('tree2', 1);                          *
* </script>                                      *
**************************************************/